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

#ifndef _BOARD_DEF_H_
#define _BOARD_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#define FPB_RA6E1
#define PCLK    100000000

#include "fsp_gpio.h"

//-------------------------------------------------------------------
// Serial - SCI
//-------------------------------------------------------------------

#define USE_SEGGER_RTT
// #define USE_SCI_SPI

#if defined(RA4M1_EK)
#if defined(SCI_CH)
#undef SCI_CH
#define SCI_CH  (0)
#endif
#if defined(SCI_TX)
#undef SCI_TX
#endif
#define SCI_TX  (P411)
#if defined(SCI_RX)
#undef SCI_RX
#endif
#define SCI_RX  (P410)
#elif defined(RA4M1_CLICKER)
#if defined(SCI_CH)
#undef SCI_CH
#endif
#define SCI_CH  (1)
#if defined(SCI_TX)
#undef SCI_TX
#endif
#define SCI_TX  (P401)
#if defined(SCI_RX)
#undef SCI_RX
#endif
#define SCI_RX  (P402)
#elif defined(RA4W1_EK)
#if defined(SCI_CH)
#undef SCI_CH
#endif
#define SCI_CH  (4)
#if defined(SCI_TX)
#undef SCI_TX
#endif
#define SCI_TX  (P205)
#if defined(SCI_RX)
#undef SCI_RX
#endif
#define SCI_RX  (P206)
#elif defined(RA6M2_EK)
#if defined(SCI_CH)
#undef SCI_CH
#endif
#define SCI_CH  (4)
#if defined(SCI_TX)
#undef SCI_TX
#endif
#define SCI_TX  (P411)
#if defined(SCI_RX)
#undef SCI_RX
#endif
#define SCI_RX  (P410)
#elif defined(RA6M5_EK)
#if defined(SCI_CH)
#undef SCI_CH
#endif
#define SCI_CH  (7)
#if defined(SCI_TX)
#undef SCI_TX
#endif
#define SCI_TX  (P613)
#if defined(SCI_RX)
#undef SCI_RX
#endif
#define SCI_RX  (P614)
#elif defined(FPB_RA6E1)
#if defined(SCI_CH)
#undef SCI_CH
#endif
#define SCI_CH  (9)
#if defined(SCI_TX)
#undef SCI_TX
#endif
#define SCI_TX  (P109)
#if defined(SCI_RX)
#undef SCI_RX
#endif
#define SCI_RX  (P110)
#else
#error "BOARD macro is not defined"
#endif

//-------------------------------------------------------------------
// SD CARD
//-------------------------------------------------------------------

#if defined(RA4M1_EK)
#define SD_SPI_CH   0
#define SD_SPI_MOSI P101
#define SD_SPI_MISO P100
#define SD_SPI_CLK  P102
#define SD_SPI_CS   P111
#elif defined(RA4M1_CLICKER)
#define SD_SPI_CH   0
#define SD_SPI_MOSI P101
#define SD_SPI_MISO P100
#define SD_SPI_CLK  P102
#define SD_SPI_CS   P302
#elif defined(RA4W1_EK)
#define SD_SPI_CH   0
#define SD_SPI_MOSI P101
#define SD_SPI_MISO P100
#define SD_SPI_CLK  P102
#define SD_SPI_CS   P204
#elif defined(RA6M2_EK)
#define SD_SPI_CH   0
#define SD_SPI_MOSI P101
#define SD_SPI_MISO P100
#define SD_SPI_CLK  P102
#define SD_SPI_CS   P111
#elif defined(RA6M5_EK)
#define SD_SPI_CH   0
#define SD_SPI_MOSI P203
#define SD_SPI_MISO P202
#define SD_SPI_CLK  P204
#define SD_SPI_CS   P301
#elif defined(FPB_RA6E1)
#define SD_SPI_CH   1
#define SD_SPI_MOSI P101
#define SD_SPI_MISO P100
#define SD_SPI_CLK  P102
#define SD_SPI_CS   P113
#else
#error "BOARD macro is not defined"
#endif
#define MMC_WP  (0)     /* no write protection */
#define MMC_CD  (1)     /* cd inserted */

//-------------------------------------------------------------------
// SPI LCD
//-------------------------------------------------------------------

#if defined(RA4M1_EK)
/*
 * PMOD A
 * 1 SS     P103
 * 2 MOSI   P101
 * 3 MISO   P100
 * 4 SCK    P102
 * 5 GND    GND
 * 6 VCC    +3V3
 * 7 INT    P104
 * 8 RESET  P107
 * 9 N/S    P111
 * 10 N/S   P112
 * 11 GND   GND
 * 12 VCC   +3V3
 *
 * PMOD B
 * 1 CTS    P403
 * 2 TXD    P401
 * 3 RXD    P400
 * 4 RTS    P402
 * 5 GND    GND
 * 6 VCC    +3V3
 */

#define HW_LCDSPI_CH    (0)
#define HW_LCDSPI_CLK   (P102)
#define HW_LCDSPI_MOSI  (P101)
#define HW_LCDSPI_MISO  (P100)
#define HW_LCDSPI_CS    (P103)
#define HW_LCDSPI_RESET (P107)
#define HW_LCDSPI_RS    (P104)

#elif defined(RA4M1_CLICKER)

#define HW_LCDSPI_CH    (0)
#define HW_LCDSPI_CLK   (P102)
#define HW_LCDSPI_MOSI  (P101)
#define HW_LCDSPI_MISO  (P100)
#define HW_LCDSPI_CS    (P103)
#define HW_LCDSPI_RESET (P407)
#define HW_LCDSPI_RS    (P104)

#elif defined(RA4W1_EK)

#define HW_LCDSPI_CH    (0)
#define HW_LCDSPI_CLK   (P102)
#define HW_LCDSPI_MOSI  (P101)
#define HW_LCDSPI_MISO  (P100)
#define HW_LCDSPI_CS    (P103)
#define HW_LCDSPI_RESET (P107)
#define HW_LCDSPI_RS    (P104)

#elif defined(RA6M2_EK)

#define HW_LCDSPI_CH    (0)
#define HW_LCDSPI_CLK   (P102)
#define HW_LCDSPI_MOSI  (P101)
#define HW_LCDSPI_MISO  (P100)
#define HW_LCDSPI_CS    (P103)
#define HW_LCDSPI_RESET (P107)
#define HW_LCDSPI_RS    (P104)

#elif defined(RA6M5_EK)

#define HW_LCDSPI_CH    (0)
#define HW_LCDSPI_CLK   (P204)
#define HW_LCDSPI_MOSI  (P203)
#define HW_LCDSPI_MISO  (P202)
#define HW_LCDSPI_CS    (P206)
#define HW_LCDSPI_RESET (P311)
#define HW_LCDSPI_RS    (P905)

#elif defined(FPB_RA6E1)

#define HW_LCDSPI_CH    (1)
#define HW_LCDSPI_CLK   (P102)
#define HW_LCDSPI_MOSI  (P101)
#define HW_LCDSPI_MISO  (P100)
#define HW_LCDSPI_CS    (P103)
#define HW_LCDSPI_RESET (P402)
#define HW_LCDSPI_RS    (P302)

#endif

//-------------------------------------------------------------------
//  LCD Module ID
//-------------------------------------------------------------------

/* LCD Model */
//#define NOKIA6100_0   0
//#define NOKIA6100_1   1
//#define T180          2
//#define M022C9340SPI  3
//#define RASPI13LCDSPI 4
//#define RASPI28LCDSPI 5
//#define KMRTM24024SPI 6

#define DEF_LCDSPI      (3)

#ifdef __cplusplus
}
#endif

#endif /* _BOARD_DEF_H_ */
