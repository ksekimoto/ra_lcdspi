/*
 * Copyright (c) 2020, Kentaro Sekimoto
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

#ifndef LCD_LCDSPI_H_
#define LCD_LCDSPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "font.h"

/* LCD SPI Channel */
#define SW_LCDSPI_CH (0xffffffff)

/* LCD Controller */
#define PCF8833  0
#define S1D15G10 1
#define ILI9341  2
#define ILI9340  3
#define ST7735   4
#define ST7789   5

/* LCD Model */
#define NOKIA6100_0   0
#define NOKIA6100_1   1
#define T180          2
#define M022C9340SPI  3
#define RASPI13LCDSPI 4
#define RASPI28LCDSPI 5
#define KMRTM24024SPI 6

// RGB 565 format x2 => RG BR GB 44 44 44 format
// v1: rrrrrggg gggbbbbb
// v2: rrrrrggg gggbbbbb
#define R4G4(v1)     ((uint8_t)(((v1 & 0xf000) >> 8) | ((v1 & 0x07e0) >> 7)))
#define B4R4(v1, v2) ((uint8_t)(((v1 & 0x1f) << 3) | (v2 >> 12)))
#define G4B4(v2)     ((uint8_t)(((v2 & 0x07e0) >> 3) | ((v2 & 0x1f) >> 1)))

/*
 * LCD SPI pin configuration
 */
typedef struct {
    uint32_t pin_clk;
    uint32_t pin_dout;
    uint32_t pin_din;
    uint32_t pin_cs;
    uint32_t pin_reset;
    uint32_t pin_rs;
} lcdspi_pins_t;

/*
 * LCD Controller information
 */
typedef struct {
    const uint8_t id;
    const uint8_t PASET;
    const uint8_t CASET;
    const uint8_t RAMWR;
} lcdspi_ctrl_info_t;

/*
 * LCD module information
 */
typedef struct {
    const char *name;
    const uint32_t lcd_info_id;
    void (*lcdspi_init)();
    void (*lcdspi_reset)();
    const uint32_t def_baud;
    const lcdspi_ctrl_info_t *lcdspi_ctrl_info;
    const uint32_t disp_wx;
    const uint32_t disp_wy;
    const uint32_t PWX;
    const uint32_t PWY;
    const uint32_t text_sx;
    const uint32_t text_sy;
} lcdspi_lcd_t;

/*
 * LCD screen information
 */
typedef struct {
    font_t *font;
    uint16_t cx;
    uint16_t cy;
    uint16_t fcol;
    uint16_t bcol;
    uint32_t unit_wx;
    uint32_t unit_wy;
} lcdspi_screen_t;

/*
 * LCD SPI information
 */
typedef struct {
    uint32_t spi_ch;
    const lcdspi_lcd_t *lcdspi_lcd;
    lcdspi_screen_t *lcdspi_screen;
    lcdspi_pins_t *lcdspi_pins;
    uint32_t baud;
    uint16_t spcmd;
    uint8_t spbr;
    uint8_t polarity;
    uint8_t phase;
} lcdspi_t;

void lcdspi_set_spi_ch(lcdspi_t *lcdspi, uint32_t spi_ch);
void lcdspi_set_pins(lcdspi_t *lcdspi, lcdspi_pins_t *lcdspi_pins);
void lcdspi_set_lcd(lcdspi_t *lcdspi, uint32_t lcd_id);
void lcdspi_set_screen(lcdspi_t *lcdspi, lcdspi_screen_t *lcdspi_screen);
void SPISW_Initialize(void);
void SPISW_Write(uint8_t dat);
void SPI_Write(uint8_t dat);
void SPISW_LCD_cmd8_0(uint8_t dat);
void SPISW_LCD_dat8_0(uint8_t dat);
void SPISW_LCD_cmd8_1(uint8_t dat);
void SPISW_LCD_dat8_1(uint8_t dat);
void SPISW_LCD_dat0_n(uint32_t size);
void SPISW_LCD_buf8_1(uint8_t *buf, uint32_t size);
void SPI_cmd_data_1(uint8_t cmd, uint8_t *data, uint32_t data_size);
void lcdspi_spi_init(lcdspi_t *lcdspi);
void lcdspi_clear(lcdspi_t *lcdspi);
void lcdspi_screen_init(lcdspi_screen_t *lcdspi_screen);
void lcdspi_init(lcdspi_t *lcdspi);
void lcdspi_deinit(lcdspi_t *lcdspi);
void lcdspi_bitbltex565(lcdspi_t *lcdspi, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t *data);
void lcdspi_bitbltex(lcdspi_t *lcdspi, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t *data);
void lcdspi_write_char_color(lcdspi_t *lcdspi, unsigned char c, uint32_t cx, uint32_t cy, uint16_t fgcol, uint16_t bgcol);
void lcdspi_write_unicode_color(lcdspi_t *lcdspi, unsigned short u, uint32_t cx, uint32_t cy, uint16_t fgcol, uint16_t bgcol);
void lcdspi_write_char(lcdspi_t *lcdspi, unsigned char c, uint32_t row, uint32_t col);
void lcdspi_write_unicode(lcdspi_t *lcdspi, unsigned short u, uint32_t row, uint32_t col);
void lcdspi_write_formatted_char(lcdspi_t *lcdspi, unsigned char ch);
void lcdspi_write_formatted_unicode(lcdspi_t *lcdspi, unsigned short u);
void lcdspi_set_font(lcdspi_t *lcdspi, font_t *font);
font_t *lcdspi_get_font(lcdspi_t *lcdspi);
unsigned short cnvUtf8ToUnicode(unsigned char *str, uint32_t *size);

#ifdef __cplusplus
}
#endif

#endif /* LCD_LCDSPI_H_ */
