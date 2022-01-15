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

/*----------------------------------------------------------------------*/
/* FAT file system sample project for FatFs            (C)ChaN, 2018    */
/*----------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include "diskio.h"
#include "hal_data.h"
#include "board_def.h"
#include "fsp_gpio.h"
#include "fsp_time.h"

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#endif

#if defined(RA4M1_CLICKER)
#define LED1_PIN    (P409)
#elif defined(RA4M1_EK)
#define LED1_PIN    (P106)
#elif defined(RA4W1_EK)
#define LED1_PIN    (P106)
#elif defined(RA6M2_EK)
#define LED1_PIN    (P106)
#elif defined(RA6M5_EK)
#define LED1_PIN    (P006)
#elif defined(FPB_RA6E1)
#define LED1_PIN    (P408)
#else
#error "BOARD macro is not defined"
#endif

extern void disk_timerproc (void);

/*---------------------------------------------*/
/* 1kHz timer process                          */
/*---------------------------------------------*/

volatile UINT Timer;
static uint32_t led_level = 0;

void SysTick_Handler (void)
{
    static uint16_t led;


    Timer++;    /* Increment performance counter */

    if (++led >= 500) {
        led = 0;
        led_level ^= (uint32_t)(-1);
        fsp_gpio_write(LED1_PIN, (bool)led_level);
    }

    disk_timerproc();   /* Disk timer process */
}

/*
 * This assembly language routine takes roughly 4 cycles per loop. 2 additional cycles
 */

__attribute__((naked)) void min_delay(__attribute__((unused)) uint32_t loop_cnt) {
    __asm volatile(
        "sw_delay_loop:         \n"
#if defined(__ICCARM__) || defined(__ARMCC_VERSION)
        "   subs r0, #1         \n"  ///< 1 cycle
#elif defined(__GNUC__)
        "   sub r0, r0, #1      \n"  ///< 1 cycle
#endif
        "   cmp r0, #0          \n"  ///< 1 cycle
/* CM0 and CM23 have a different instruction set */
#if defined(__CORE_CM0PLUS_H_GENERIC) || defined(__CORE_CM23_H_GENERIC)
        "   bne sw_delay_loop   \n"  ///< 2 cycles
#else
        "   bne.n sw_delay_loop \n"  ///< 2 cycles
#endif
        "   bx lr               \n");  ///< 2 cycles
}

void mdelay(uint32_t ms) {
    while (ms-- > 0) {
        min_delay(1000 * 48 / 4);
    }
}
