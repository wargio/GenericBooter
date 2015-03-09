/*
 * Copyright (c) 2013, Brian McKenzie <mckenzba@gmail.com>
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

#ifndef PL011_H
#define PL011_H

/* PL011 registers */
#define PL011_DR         0x000
#define PL011_RSR_ECR    0x004
#define PL011_DMAWM      0x008
#define PL011_TIMEOUT    0x00C
#define PL011_FR         0x018
#define PL011_LCRH_RX    0x01C
#define PL011_ILPR       0x020
#define PL011_IBRD       0x024
#define PL011_FBRD       0x028
#define PL011_LCRH_TX    0x02C
#define PL011_CR         0x030
#define PL011_IFLS       0x034
#define PL011_IMSC       0x038
#define PL011_RIS        0x03C
#define PL011_MIS        0x040
#define PL011_ICR        0x044
#define PL011_DMACR      0x048
#define PL011_XFCR       0x050
#define PL011_XON1       0x054
#define PL011_XON2       0x058
#define PL011_XOFF1      0x05C
#define PL011_XOFF2      0x060
#define PL011_ITCR       0x080
#define PL011_ITIP       0x084
#define PL011_ITOP       0x088
#define PL011_YDR        0x08C
#define PL011_ABCR       0x100
#define PL011_ABSR       0x104
#define PL011_ABFMT      0x108
#define PL011_ABDR       0x150
#define PL011_ABDFR      0x154
#define PL011_ABMR       0x158
#define PL011_ABIMSC     0x15C
#define PL011_ABRIS      0x160
#define PL011_ABMIS      0x164
#define PL011_ABICR      0x168
#define PL011_IDPROD     0xFD8
#define PL011_IDPROV     0xFDC
#define PL011_PERIPH0    0xFE0
#define PL011_PERIPH1    0xFE4
#define PL011_PERIPH2    0xFE8
#define PL011_PERIPH3    0xFEC
#define PL011_PCELLID0   0xFF0
#define PL011_PCELLID1   0xFF4
#define PL011_PCELLID2   0xFF8
#define PL011_PCELLID3   0xFFC

/* PL011 status bits */
#define PL011_FR_TXFE_BIT 0x080
#define PL011_FR_RXFF_BIT 0x040
#define PL011_FR_TXFF_BIT 0x020
#define PL011_FR_RXFE_BIT 0x010
#define PL011_ICR_DISA    0xFFF
#define PL011_CR_DISA     0x000
#define PL011_ICR_CLRP    0x7FF

/* PL011 control bits */
#define PL011_LCRH_RXFE  0x010
#define PL011_LCRH_TXFE  0x010
#define PL011_CR_RXE     0x100
#define PL011_CR_TXE     0x080
#define PL011_CR_UEN     0x001
#define PL011_CR_RTS     0x800
#define PL011_CR_OSVFACT 0x008
#define PL011_IMSC_RXMS  0x010
#define PL011_IMSC_TXMS  0x020

/* PL011 LCRH word lengths */
#define PL011_LCRH_8WL   0x060
#define PL011_LCRH_7WL   0x040
#define PL011_LCRH_6WL   0x020
#define PL011_LCRH_5WL   0x000

#endif /* !PL011_H */
