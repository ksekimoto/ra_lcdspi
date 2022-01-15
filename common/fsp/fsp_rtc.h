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

#ifndef FSP_RTC_H_
#define FSP_RTC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define RTC_PERIOD_MINUTE   0x00
#define RTC_PERIOD_SECOND   0x01

typedef struct ra_rtc {
    uint16_t  year;
    uint8_t   month;
    uint8_t   date;
    uint8_t   weekday;
    uint8_t   hour;
    uint8_t   minute;
    uint8_t   second;
} ra_rtc_t;

typedef void (*ra_rtc_cb_t)(void);

uint32_t fsp_rtc_get_year(void);
uint32_t fsp_rtc_get_month(void);
uint32_t fsp_rtc_get_date(void);
uint32_t fsp_rtc_get_hour(void);
uint32_t fsp_rtc_get_minute(void);
uint32_t fsp_rtc_get_second(void);
uint32_t fsp_rtc_get_weekday(void);
bool fsp_rtc_init(void);
bool fsp_rtc_deinit(void);
bool fsp_rtc_set_time(ra_rtc_t *time);
bool fsp_rtc_get_time(ra_rtc_t *time);

#ifdef __cplusplus
}
#endif

#endif /* FSP_RTC_H_ */
