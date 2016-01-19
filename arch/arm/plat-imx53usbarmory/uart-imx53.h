/*
 * Copyright (c) 2015, Vince Cali. <0x56.0x69.0x6e.0x63.0x65@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  3. Neither the name of the organization nor the names of its contributors may
 *     be used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef UART_IMX53_H
#define UART_IMX53_H

/*
 * i.MX53 Multimedia Applications Processor Reference Manual; iMX53RM.pdf, p. 4668
 */

/* i.MX53 UART registers */
#define IMX53_UART_URXD         0
#define IMX53_UART_UTXD         0x40
#define IMX53_UART_UCR1         0x80
#define IMX53_UART_UCR2         0x84
#define IMX53_UART_UCR3         0x88
#define IMX53_UART_UCR4         0x8C
#define IMX53_UART_UFCR         0x90
#define IMX53_UART_USR1         0x94
#define IMX53_UART_USR2         0x98
#define IMX53_UART_UESC         0x9C
#define IMX53_UART_UTIM         0xA0
#define IMX53_UART_UBIR         0xA4
#define IMX53_UART_UBMR         0xA8
#define IMX53_UART_UBRC         0xAC
#define IMX53_UART_ONEMS        0xB0
#define IMX53_UART_UTS          0xB4

/* i.MX53 UART status bits */
#define IMX53_UART_USR1_TRDY    (1 << 13)
#define IMX53_UART_USR2_TXFE    (1 << 14)
#define IMX53_UART_UTS_TXEMPTY  (1 << 6)
#define IMX53_UART_UTS_TXFULL   (1 << 4)

/* i.MX53 UART control bits */
#define IMX53_UART_UCR1_UARTEN  1

#endif /* UART_IMX53_H */
