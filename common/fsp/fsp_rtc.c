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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "hal_data.h"
#include "fsp_rtc.h"

#if !defined(USE_FSP_RTC)
#define USE_FSP_RTC
#endif

#if defined(USE_FSP_RTC)

#define RESET_VALUE (0x00)

static rtc_time_t g_set_time =  {
    .tm_hour    =  RESET_VALUE,
    .tm_isdst   =  RESET_VALUE,
    .tm_mday    =  RESET_VALUE,
    .tm_min     =  RESET_VALUE,
    .tm_mon     =  RESET_VALUE,
    .tm_sec     =  RESET_VALUE,
    .tm_wday    =  RESET_VALUE,
    .tm_yday    =  RESET_VALUE,
    .tm_year    =  RESET_VALUE,
};

static rtc_time_t g_present_time =  {
    .tm_hour    =  RESET_VALUE,
    .tm_isdst   =  RESET_VALUE,
    .tm_mday    =  RESET_VALUE,
    .tm_min     =  RESET_VALUE,
    .tm_mon     =  RESET_VALUE,
    .tm_sec     =  RESET_VALUE,
    .tm_wday    =  RESET_VALUE,
    .tm_yday    =  RESET_VALUE,
    .tm_year    =  RESET_VALUE,
};

bool fsp_rtc_set_time(ra_rtc_t *time) {
    fsp_err_t err = FSP_SUCCESS;
    g_set_time.tm_hour = (int)time->hour;
    g_set_time.tm_mday = (int)time->date;
    g_set_time.tm_min = (int)time->minute;
    g_set_time.tm_mon = (int)time->month;
    g_set_time.tm_sec = (int)time->second;
    g_set_time.tm_wday = (int)time->weekday;
    g_set_time.tm_year = (int)time->year - 1900;
    err = R_RTC_CalendarTimeSet(&g_rtc0_ctrl, &g_set_time);
    if (FSP_SUCCESS != err) {
        return false;
    } else {
        return true;
    }
}

bool fsp_rtc_get_time(ra_rtc_t *time) {
    fsp_err_t err = FSP_SUCCESS;
    err = R_RTC_CalendarTimeGet(&g_rtc0_ctrl, &g_present_time);
    /* Handle error */
    if (FSP_SUCCESS != err) {
        return false;
    }
    time->date = (uint8_t)g_present_time.tm_mday;
    time->hour = (uint8_t)g_present_time.tm_hour;
    time->minute = (uint8_t)g_present_time.tm_min;
    time->month = (uint8_t)g_present_time.tm_mon;
    time->second = (uint8_t)g_present_time.tm_sec;
    time->weekday = (uint8_t)g_present_time.tm_wday;
    time->year = (uint16_t)(g_present_time.tm_year + 1900);
    return true;
}

bool fsp_rtc_init(void) {
    fsp_err_t err = FSP_SUCCESS;
    err = R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    if (FSP_SUCCESS != err) {
        return false;
    } else {
        return true;
    }
}

bool fsp_rtc_deinit(void) {
    fsp_err_t err = FSP_SUCCESS;
    err = R_RTC_Close(&g_rtc0_ctrl);
    if (FSP_SUCCESS != err) {
        return false;
    } else {
        return true;
    }
}

#else

// ToDo: implementation

#endif
