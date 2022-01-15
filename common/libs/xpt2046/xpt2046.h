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

#ifndef LIBS_XPT2046_XPT2046_H_
#define LIBS_XPT2046_XPT2046_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define XPT2046_BAUDRATE   1000000
#define XPT2046_AVG 4
#define CMD_X_READ  0b10010000
#define CMD_Y_READ  0b11010000

typedef struct _xpt2046_t {
    uint32_t baud;
    uint32_t ch;
    uint32_t mosi;
    uint32_t miso;
    uint32_t sck;
    uint32_t cs;
    uint32_t irq;
    uint16_t spcmd;
    uint8_t spbr;

    int16_t x_min;
    int16_t y_min;
    int16_t x_max;
    int16_t y_max;
    bool x_inv;
    bool y_inv;
    bool xy_swap;

    int16_t avg_buf_x[XPT2046_AVG];
    int16_t avg_buf_y[XPT2046_AVG];
    uint8_t avg_last;
} xpt2046_t;

static uint8_t xpt2046_spi_xchg(xpt2046_t *xpt, uint8_t dat);
static void xpt2046_corr(xpt2046_t *xpt, int16_t *x, int16_t *y);
static void xpt2046_avg(xpt2046_t *xpt, int16_t *x, int16_t *y);
bool xpt2046_read(xpt2046_t *xpt, uint16_t *data_x, uint16_t *data_y);
void xpt2046_init(xpt2046_t *xpt, uint32_t ch, uint32_t mosi, uint32_t miso, uint32_t sck, uint32_t cs, uint32_t irq, uint32_t baud);
void xpt2046_deinit(uint32_t ch);

#ifdef __cplusplus
}
#endif

#endif /* LIBS_XPT2046_XPT2046_H_ */
