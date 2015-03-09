/*
 * Copyright 2013, winocm. <winocm@icloud.com>
 * Copyright 2015, Brian McKenzie. <mckenzba@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   If you are going to use this software in any form that does not involve
 *   releasing the source to this project or improving it, let me know beforehand.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "pl011.h"
#include "genboot.h"
#include <stdarg.h>
#include <stdio.h>

/*
 * Generic 32/26/8-bit memory I/O functions.
 */
static inline void regwrite32(uint32_t addr, uint32_t data)
{
    *(volatile uint32_t *)addr = data;
}

static inline uint32_t regread32(uint32_t addr)
{
    uint32_t value = *(volatile uint32_t *)addr;

    return value;
}

/*
 * Macros to clear and set data in registers
 */
#define regset32(addr, data) \
    regwrite32(addr, (regread32(addr) | data))

#define regclear32(addr, data) \
    regwrite32(addr, (regread32(addr) & ~data))

/*
 * Memory barrier.
 */
#define barrier() \
    __asm__ __volatile__("": : :"memory");


/*
 * Delay loop.
 */
static inline void delay(uint32_t count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
         : : [count]"r"(count) : "cc");
}

/*
 * Peripheral definitions
 */
#define BCM2836_PERIPH_BASE     0x3F000000
#define BCM2836_GPIO_BASE       (BCM2836_PERIPH_BASE + 0x200000)
#define BCM2836_UART0_BASE      (BCM2836_PERIPH_BASE + 0x201000)

/*
 * GPIO registers
 */
#define BCM_GPIO_GPPUD      0x94
#define BCM_GPIO_GPPUDCLK0  0x98

/*
 * GPIO pin macro
 */
 #define BCM_GPIO_PIN(p)    (1 << p)

/*
 * Serial uart clock speed and baud rate
 */
#define BCM2836_UART_CLOCK  3000000
#define UART_BAUD           115200


static int inited_printf = 1;

void uart_putchar(int c)
{
    if (!inited_printf)
        return;

    if (c == '\n')
        uart_putchar('\r');

    while (regread32(BCM2836_UART0_BASE + PL011_FR) & PL011_FR_TXFF_BIT)
        barrier();

    regwrite32((BCM2836_UART0_BASE + PL011_DR), c);

    return;
}

/**
 * uart_getc
 *
 * Get a character from system input.
 */
int uart_getchar(void)
{
    return 'X';
}

static void putc_wrapper(void *p, char c)
{
    uart_putchar(c);

    return;
}

/**
 * init_debug
 *
 * Start debugging subsystems.
 */
void init_debug(void)
{
    int div, frac, rem;

    /* Wait until the uart tx fifo is empty before proceeding */
    while (regread32(BCM2836_UART0_BASE + PL011_FR) & PL011_FR_TXFF_BIT)
        barrier();

    /* Turn off the uart */
    regwrite32((BCM2836_UART0_BASE + PL011_CR), PL011_CR_DISA);

    /* Disable pull up/down for all pins and delay for 150 cycles. */
    regwrite32((BCM2836_GPIO_BASE + BCM_GPIO_GPPUD), 0x00000000);
    delay(150);

    /* Disable pull up/down for pins 14 and 15 and delay for 150 cycles. */
    regset32((BCM2836_GPIO_BASE + BCM_GPIO_GPPUD), BCM_GPIO_PIN(14));
    regset32((BCM2836_GPIO_BASE + BCM_GPIO_GPPUD), BCM_GPIO_PIN(15));
    delay(150);

    /* Write 0 to GPPUDCLK0 to make it take effect. */
    regwrite32((BCM2836_GPIO_BASE + BCM_GPIO_GPPUDCLK0), 0x00000000);

    /* Clear pending interrupts. */
    regwrite32((BCM2836_UART0_BASE + PL011_ICR), PL011_ICR_CLRP);

    /* Compute the integer and fraction divisor */
    div = BCM2836_UART_CLOCK / (16 * UART_BAUD);
    rem = BCM2836_UART_CLOCK % (16 * UART_BAUD);
    frac = ((8 * rem / UART_BAUD) >> 1);
    frac += ((8 * rem / UART_BAUD) & 1);

    /* Set the integer and fraction divisor */
    regwrite32((BCM2836_UART0_BASE + PL011_IBRD), div);
    regwrite32((BCM2836_UART0_BASE + PL011_FBRD), frac);

    /* Set rx and tx interrupt masks. */
    regset32((BCM2836_UART0_BASE + PL011_IMSC), (PL011_IMSC_RXMS));
    regset32((BCM2836_UART0_BASE + PL011_IMSC), (PL011_IMSC_TXMS));

    /* Apply uart settings. */
    regset32((BCM2836_UART0_BASE + PL011_LCRH_RX), PL011_LCRH_8WL);
    regset32((BCM2836_UART0_BASE + PL011_LCRH_RX), PL011_LCRH_RXFE);
    regset32((BCM2836_UART0_BASE + PL011_LCRH_TX), PL011_LCRH_8WL);
    regset32((BCM2836_UART0_BASE + PL011_LCRH_TX), PL011_LCRH_TXFE);

    /* Turn on the uart. */
    regset32((BCM2836_UART0_BASE + PL011_CR), PL011_CR_UEN);
    regset32((BCM2836_UART0_BASE + PL011_CR), PL011_CR_RXE);
    regset32((BCM2836_UART0_BASE + PL011_CR), PL011_CR_TXE);

    init_printf(NULL, putc_wrapper);
    printf("debug_init()\n");

    return;
}

void __assert_func(const char *file, int line, const char *method,
                   const char *expression)
{
    panic("Assertion failed in file %s, line %d. (%s)\n",
          file, line, expression);
    while (1) ;
}

/**
 * puts
 */
int puts(const char* fmt)
{
    while(*fmt != '\0') {
        uart_putchar(*fmt);
        fmt++;
    }
    uart_putchar('\n');
    return 0;
}

/**
 * panic
 *
 * Halt the system and explain why.
 */
#undef panic
void panic(const char *panicStr, ...)
{
    void *caller = __builtin_return_address(0);

    /* Prologue */
    printf("panic(caller 0x%08x): ", caller);

    /* Epilogue */
    va_list valist;
    va_start(valist, panicStr);
    vprintf((char *)panicStr, valist);
    va_end(valist);

    /* We are hanging here. */
    printf("\npanic: we are hanging here...\n");

    /* Halt */
    _locore_halt_system();
}
