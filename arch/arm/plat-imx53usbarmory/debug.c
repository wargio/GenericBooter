/*
 * Copyright 2013, winocm. <winocm@icloud.com>
 * Copyright 2015, Brian McKenzie. <mckenzba@gmail.com>
 * Copyright 2015, Vince Cali. <0x56.0x69.0x6e.0x63.0x65@gmail.com>
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

#include "uart-imx53.h"
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
 * Peripheral definitions
 */
#define IMX53_SDRAM_BASE     0x70000000  // iMX53RM.pdf, Page:523: i.MX53 supports 2G, armory has 512M
#define IMX53_IOMUXC_BASE    0x53FA8000
#define IMX53_UART1_BASE     (IMX53_IOMUXC_BASE + 0x14000)  // iMX53RM.pdf, Page:4668: 0x53FBC000

/*
 * Serial uart clock speed and baud rate
 */
#define UART_BAUD            115200


static int inited_printf = 1;
/**
 * uart_putchar
 *
 * Put a character to the system console.
 */
void uart_putchar(int c)
{
    if (!inited_printf)
        return;

    if (c == '\n')
        uart_putchar('\r');

    while (regread32(IMX53_UART1_BASE + IMX53_UART_UTS) & IMX53_UART_UTS_TXFULL)
        /* TxFIFO is full, wait */
        barrier();

    regwrite32((IMX53_UART1_BASE + IMX53_UART_UTXD), c);
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
}

/**
 * init_debug
 *
 * Start debugging subsystems.
 */
void init_debug(void)
{
    init_printf(NULL, putc_wrapper);
    printf("debug_init()\n");
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
