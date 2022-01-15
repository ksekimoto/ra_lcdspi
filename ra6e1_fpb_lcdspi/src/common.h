/*
 * Copyright (c) 2021, Kentaro Sekimoto
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  -Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *  -Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef COMMON_H_
#define COMMON_H_

#define SCI_CH      9
#define SCI_BAUD    115200
#define UART_CH     SCI_CH
#define UART_TxStr  ra_sci_tx_str
#if defined(USE_DBG_PRINT)
#if !defined(DEBUG_CH)
#define DEBUG_CH SCI_CH
#endif
#endif

#define RA_PRI_SYSTICK  (0)
#define RA_PRI_UART     (1)
#define RA_PRI_SDIO     (4)
#define RA_PRI_DMA      (5)
#define RA_PRI_FLASH    (6)
#define RA_PRI_OTG_FS   (6)
#define RA_PRI_OTG_HS   (6)
#define RA_PRI_TIM5     (6)
#define RA_PRI_CAN      (7)
#define RA_PRI_SPI      (8)
#define RA_PRI_I2C      (8)
#define RA_PRI_TIMX     (13)
#define RA_PRI_EXTINT   (14)
#define RA_PRI_PENDSV   (15)
#define RA_PRI_RTC_WKUP (15)

#if defined(USE_DBG_PRINT)
#include "debug_printf.h"
#endif

#include "board_def.h"
#include "fsp_gpio.h"
#include "fsp_rtc.h"
#include "fsp_sci.h"
#include "fsp_spi.h"
#include "fsp_time.h"

#endif /* COMMON_H_ */
