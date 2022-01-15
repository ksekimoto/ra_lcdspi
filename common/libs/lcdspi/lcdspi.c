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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#if defined(RZA2M)
#include "systick.h"
#endif

#include "PCF8833.h"
#include "S1D15G10.h"
#include "ILI9341.h"
#include "ILI9340.h"
#include "ST7735.h"
#include "ST7789.h"
#include "font.h"
#if defined(RX65N)
#include "rx65n_gpio.h"
#include "rx65n_spi.h"
#include "rx65n_timer.h"
#include "rx65n_utils.h"
#endif

#if defined(RZA2M)
#include "rza2m_gpio.h"
#include "rza2m_spi.h"
#endif
#include "jpeg.h"
#include "lcdspi.h"
#include "common.h"

#if defined(USE_SCI_SPI)
#include "fsp_sci_spi.h"
#else
#include "fsp_spi.h"
#endif

#if defined(RZA2M)
#define GPIO_SET_OUTPUT rz_gpio_mode_output
#define GPIO_SET_INPUT  rz_gpio_mode_input
#define GPIO_WRITE      rz_gpio_write
#define SPI_WRITE_BYTE  rz_spi_write_byte
#define SPI_INIT        rz_spi_init
#define SPI_GET_CONF    rz_spi_get_conf
#define SPI_START_XFER  rz_spi_start_xfer
#define SPI_END_XFER    rz_spi_end_xfer
#define SPI_TRANSFER    rz_spi_transfer
#elif defined(RX63N) | defined(RX65N)
#define GPIO_SET_OUTPUT rx_gpio_mode_output
#define GPIO_SET_INPUT  rx_gpio_mode_input
#define GPIO_WRITE      rx_gpio_write
#define SPI_WRITE_BYTE  rx_spi_write_byte
#define SPI_INIT        rx_spi_init
#define SPI_GET_CONF    rx_spi_get_conf
#define SPI_START_XFER  rx_spi_start_xfer
#define SPI_END_XFER    rx_spi_end_xfer
#define SPI_TRANSFER    rx_spi_transfer
#else
#define GPIO_SET_OUTPUT fsp_gpio_mode_output
#define GPIO_SET_INPUT  fsp_gpio_mode_input
#define GPIO_WRITE      fsp_gpio_write
#if defined(USE_SCI_SPI)
#define SPI_WRITE_BYTE  fsp_sci_spi_write_byte
#define SPI_INIT        fsp_sci_spi_init
#define SPI_GET_CONF    fsp_sci_spi_get_conf
#define SPI_START_XFER  fsp_sci_spi_start_xfer
#define SPI_END_XFER    fsp_sci_spi_end_xfer
#define SPI_TRANSFER    fsp_sci_spi_transfer
#else
#define SPI_WRITE_BYTE  fsp_spi_write_byte
#define SPI_INIT        fsp_spi_init
#define SPI_GET_CONF    fsp_spi_get_conf
#define SPI_START_XFER  fsp_spi_start_xfer
#define SPI_END_XFER    fsp_spi_end_xfer
#define SPI_TRANSFER    fsp_spi_transfer
#endif
#endif

#if defined(USE_DBG_PRINT)
#define DEBUG_LCDSPI
#endif

//#define LCDSPI_OPTIMIZE

#ifdef HIGH
#undef HIGH
#endif
#define HIGH 1
#ifdef LOW
#undef LOW
#endif
#define LOW 0

static void PCF8833_Reset();
static void PCF8833_Initialize();
static void S1D15G10_Reset();
static void S1D15G10_Initialize();
static void ILI9341_Reset();
static void ILI9341_Initialize();
static void ILI9340_Reset();
static void ILI9340_Initialize();
static void ST7735_Reset();
static void ST7735_Initialize();
static void ST7789_Reset();
static void ST7789_Initialize();

#ifdef PIN_NONE
#undef PIN_NONE
#endif
#define PIN_NONE 0xffff

#ifndef LCDSPI_DEF_ID
#define LCDSPI_DEF_ID KMRTM24024SPI
#endif

static const lcdspi_pins_t pins_def = {
#if defined(HW_LCDSPI_CLK)
    (uint32_t)HW_LCDSPI_CLK,
#else
    (uint32_t)PIN_NONE,
#endif
#if defined(HW_LCDSPI_MOSI)
    (uint32_t)HW_LCDSPI_MOSI,
#else
    (uint32_t)PIN_NONE,
#endif
#if defined(HW_LCDSPI_MISO)
    (uint32_t)HW_LCDSPI_MISO,
#else
    (uint32_t)PIN_NONE,
#endif
#if defined(HW_LCDSPI_CS)
    (uint32_t)HW_LCDSPI_CS,
#else
    (uint32_t)PIN_NONE,
#endif
#if defined(HW_LCDSPI_RESET)
    (uint32_t)HW_LCDSPI_RESET,
#else
    (uint32_t)PIN_NONE,
#endif
#if defined(HW_LCDSPI_RS)
    (uint32_t)HW_LCDSPI_RS,
#else
    (uint32_t)PIN_NONE,
#endif
};

#if defined(HW_LCDSPI_CH)
#define LCDSPI_CH HW_LCDSPI_CH
#else
#define LCDSPI_CH SW_LCDSPI_CH
#endif
static volatile uint32_t g_spi_ch = SW_LCDSPI_CH;

static __attribute__((section(".data"))) lcdspi_pins_t pins = (lcdspi_pins_t)pins_def;

/*
 * const uint8_t id;
 * const uint8_t PASET;
 * const uint8_t CASET;
 * const uint8_t RAMWR;
 */
static const lcdspi_ctrl_info_t lcdspi_ctrl_PCF8833 = {
    PCF8833, PCF8833_PASET, PCF8833_CASET, PCF8833_RAMWR
};

static const lcdspi_ctrl_info_t lcdspi_ctrl_S1D15G10 = {
    S1D15G10, S1D15G10_PASET, S1D15G10_CASET, S1D15G10_RAMWR
};

static const lcdspi_ctrl_info_t lcdspi_ctrl_ST7735 = {
    ST7735, ST7735_PASET, ST7735_CASET, ST7735_RAMWR
};

static const lcdspi_ctrl_info_t lcdspi_ctrl_ILI9341 = {
    ILI9341, ILI9341_PASET, ILI9341_CASET, ILI9341_RAMWR
};

static const lcdspi_ctrl_info_t lcdspi_ctrl_ILI9340 = {
    ILI9340, ILI9340_PASET, ILI9340_CASET, ILI9340_RAMWR
};

static const lcdspi_ctrl_info_t lcdspi_ctrl_ST7789 = {
    ST7789, ST7789_PASET, ST7789_CASET, ST7789_RAMWR
};

/*
 *  char *name;
 *  int lcd_info_id;
 *  void (*lcdspi_init)();
 *  void (*lcdspi_reset)();
 *  int def_baud;
 *  lcdspi_ctrl_info_t *lcdspi_ctrl_info;
 *  int _disp_wx;
 *  int _disp_wy;
 *  int _PWX;
 *  int _PWY;
 *  int _text_sx;
 *  int _text_sy;
 */

static const lcdspi_lcd_t lcdspi_lcd_NOKIA6100_0 = {
    (const char *)"NOKIA6100_0",
    NOKIA6100_0,
    PCF8833_Initialize,
    PCF8833_Reset,
    2000000,
    &lcdspi_ctrl_PCF8833,
    128,
    128,
    132,
    132,
    2,
    2,
};

static const lcdspi_lcd_t lcdspi_lcd_NOKIA6100_1 = {
    (const char *)"NOKIA6100_1",
    NOKIA6100_1,
    S1D15G10_Initialize,
    S1D15G10_Reset,
    2000000,
    &lcdspi_ctrl_S1D15G10,
    128,
    128,
    132,
    132,
    2,
    2,
};

static const lcdspi_lcd_t lcdspi_lcd_T180 = {
    (const char *)"AITENDO_T180",
    T180,
    ST7735_Initialize,
    ST7735_Reset,
    2000000,
    &lcdspi_ctrl_ST7735,
    128,
    192,
    128,
    192,
    0,
    0,
};

static const lcdspi_lcd_t lcdspi_lcd_M022C9340SPI = {
    (const char *)"AITENDO_M022C9340SPI",
    M022C9340SPI,
    ILI9340_Initialize,
    ILI9340_Reset,
    10000000,
    &lcdspi_ctrl_ILI9340,
    240,
    320,
    240,
    320,
    0,
    0,
};

static const lcdspi_lcd_t lcdspi_lcd_13LCDSPI = {
    (const char *)"RASPI13LCDSPI",
    RASPI13LCDSPI,
    ST7789_Initialize,
    ST7789_Reset,
    24000000,
    &lcdspi_ctrl_ST7789,
    240,
    240,
    240,
    240,
    0,
    0,
};

static const lcdspi_lcd_t lcdspi_lcd_28LCDSPI = {
    (const char *)"Adafruit_28_320x240",
    RASPI28LCDSPI,
    ILI9341_Initialize,
    ILI9341_Reset,
    12000000,
    &lcdspi_ctrl_ILI9341,
    240,
    320,
    240,
    320,
    0,
    0,
};

static const lcdspi_lcd_t lcdspi_lcd_KMRTM24024SPI = {
    (const char *)"KMRTM24024SPI",
    KMRTM24024SPI,
    ILI9341_Initialize,
    ILI9341_Reset,
    10000000,
    &lcdspi_ctrl_ILI9341,
    240,
    320,
    240,
    320,
    0,
    0,
};

const lcdspi_lcd_t *lcdspi_all[] = {
    &lcdspi_lcd_NOKIA6100_0,
    &lcdspi_lcd_NOKIA6100_1,
    &lcdspi_lcd_T180,
    &lcdspi_lcd_M022C9340SPI,
    &lcdspi_lcd_13LCDSPI,
    &lcdspi_lcd_28LCDSPI,
    &lcdspi_lcd_KMRTM24024SPI,
};
#define LCDSPI_MOD_SIZE (sizeof(lcdspi_all) / sizeof(lcdspi_lcd_t *))

/*
 *  int spi_ch;
 *  const lcdspi_lcd_t *lcdspi_lcd;
 *  lcdspi_screen_t *lcdspi_screen;
 *  lcdspi_pins_t *lcdspi_pins;
 *  uint32_t baud;
 *  uint16_t spcmd;
 *  uint8_t spbr;
 */

static __attribute__((section(".data"))) lcdspi_screen_t m_screen;
__attribute__((section(".data"))) lcdspi_t m_lcdspi = {
    LCDSPI_CH,
    &lcdspi_lcd_KMRTM24024SPI,
    &m_screen,
    &pins,
    lcdspi_lcd_KMRTM24024SPI.def_baud,
    0,
    0,
    0,
    0
};

static void SPIHW_SetPinMode(void);
static void SPISW_SetPinMode(void);
static void lcdspi_spi_start_xfer(lcdspi_t *lcdspi);
static void lcdspi_spi_end_xfer(lcdspi_t *lcdspi);
static void PCF8833_Reset(void);
static void PCF8833_Initialize(void);
static void S1D15G10_Reset(void);
static void S1D15G10_Initialize(void);
static void ST7735_Reset(void);
static void ST7735_Initialize(void);
static void ILI9341_Reset(void);
static void ILI9341_Initialize(void);
static void ILI9340_Reset(void);
static void ILI9340_Initialize(void);
static void ST7789_Reset(void);
static void ST7789_Initialize(void);
static void ILI9340_addrset(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

static void delay_ms(volatile uint32_t n) {
    mdelay(n);
}

void lcdspi_set_spi_ch(lcdspi_t *lcdspi, uint32_t spi_ch) {
    lcdspi->spi_ch = spi_ch;
}

void lcdspi_set_pins(lcdspi_t *lcdspi, lcdspi_pins_t *lcdspi_pins) {
    if (lcdspi_pins == NULL) {
        lcdspi->lcdspi_pins = &pins;
    } else {
        lcdspi->lcdspi_pins = lcdspi_pins;
    }
}

void lcdspi_set_lcd(lcdspi_t *lcdspi, uint32_t lcd_id) {
    if (lcd_id < LCDSPI_MOD_SIZE) {
        lcdspi->lcdspi_lcd = lcdspi_all[lcd_id];
    } else {
        lcdspi->lcdspi_lcd = lcdspi_all[LCDSPI_DEF_ID];
    }
    lcdspi->baud = lcdspi->lcdspi_lcd->def_baud;
}

void lcdspi_set_screen(lcdspi_t *lcdspi, lcdspi_screen_t *lcdspi_screen) {
    if (lcdspi_screen == NULL) {
        lcdspi->lcdspi_screen = &m_screen;
    } else {
        lcdspi->lcdspi_screen = lcdspi_screen;
    }
}

static void SPIHW_SetPinMode(void) {
    // ToDo: set peripheral mode for spi pins
}

static void SPISW_SetPinMode(void) {
    // ToDo: set gpio mode for spi pins
}

void SPISW_Initialize() {
    if (pins.pin_cs != PIN_NONE) {
        GPIO_SET_OUTPUT(pins.pin_cs);
        GPIO_WRITE(pins.pin_cs, LOW);
    }
    if (pins.pin_reset != PIN_NONE) {
        GPIO_SET_OUTPUT(pins.pin_reset);
    }
    if (pins.pin_rs != PIN_NONE) {
        GPIO_SET_OUTPUT(pins.pin_rs);
    }
    if (pins.pin_clk != PIN_NONE) {
        GPIO_SET_OUTPUT(pins.pin_clk);
    }
    if (pins.pin_dout != PIN_NONE) {
        GPIO_SET_OUTPUT(pins.pin_dout);
    }
    if (pins.pin_din != PIN_NONE) {
        GPIO_SET_INPUT(pins.pin_din);
    }
}

#if defined(LCDSPI_OPTIMIZE)
void SPISW_Write(uint8_t dat) {
    uint8_t i = 8;
    uint32_t port_clk = GPIO_PORT(pins.pin_clk);
    uint32_t bit_clk = GPIO_BIT(pins.pin_clk);
    uint32_t port_dout = GPIO_PORT(pins.pin_dout);
    uint32_t bit_dout = GPIO_BIT(pins.pin_dout);
    uint32_t irq_state = disable_irq();
    while (i-- > 0) {
#if defined(LCDSPI_OPTIMIZE)
        if (dat & 0x80) {
            bit_set((uint8_t *)_PPODR(port_dout), bit_dout);
        } else {
            bit_clr((uint8_t *)_PPODR(port_dout), bit_dout);
        }
        bit_clr((uint8_t *)_PPODR(port_clk), bit_clk);
        bit_set((uint8_t *)_PPODR(port_clk), bit_clk);
        dat <<= 1;
#else
        uint8_t value = (dat & 0x80) ? 1 : 0;
        GPIO_WRITE(pins.pin_dout, value);
        GPIO_WRITE(pins.pin_clk, LOW);
        GPIO_WRITE(pins.pin_clk, HIGH);
        dat <<= 1;
#endif
    }
    enable_irq(irq_state);
}
#else
void SPISW_Write(uint8_t dat) {
    uint8_t i = 8;
    uint8_t value;
    //uint32_t irq_state = ra_disable_irq();
    while (i-- > 0) {
        value = (dat & 0x80) ? 1 : 0;
        GPIO_WRITE(pins.pin_dout, value);
        GPIO_WRITE(pins.pin_clk, LOW);
        GPIO_WRITE(pins.pin_clk, HIGH);
        dat = (uint8_t)(dat << 1);
    }
    //ra_enable_irq(irq_state);
}
#endif

void SPI_Write(uint8_t dat) {
    if (g_spi_ch != SW_LCDSPI_CH) {
        SPIHW_SetPinMode();
        SPI_WRITE_BYTE((uint32_t)g_spi_ch, dat);
        SPISW_SetPinMode();
    } else {
        SPISW_Write(dat);
    }
}

void SPISW_LCD_cmd8_0(uint8_t dat) {
    // Enter command mode: SDATA=LOW at rising edge of 1st SCLK
    GPIO_WRITE(pins.pin_cs, LOW);
    GPIO_WRITE(pins.pin_dout, LOW);
    GPIO_WRITE(pins.pin_clk, LOW);
    GPIO_WRITE(pins.pin_clk, HIGH);
    SPI_Write(dat);
    GPIO_WRITE(pins.pin_cs, HIGH);
}

void SPISW_LCD_dat8_0(uint8_t dat) {
    // Enter data mode: SDATA=HIGH at rising edge of 1st SCLK
    GPIO_WRITE(pins.pin_cs, LOW);
    GPIO_WRITE(pins.pin_dout, HIGH);
    GPIO_WRITE(pins.pin_clk, LOW);
    GPIO_WRITE(pins.pin_clk, HIGH);
    SPI_Write(dat);
    GPIO_WRITE(pins.pin_cs, HIGH);
}

void SPISW_LCD_cmd8_1(uint8_t dat) {
    // Enter command mode: RS=LOW at rising edge of 1st SCLK
    GPIO_WRITE(pins.pin_cs, LOW);
    GPIO_WRITE(pins.pin_rs, LOW);
    SPI_Write(dat);
    GPIO_WRITE(pins.pin_cs, HIGH);
}

void SPISW_LCD_dat8_1(uint8_t dat) {
    // Enter data mode: RS=HIGH at rising edge of 1st SCLK
    GPIO_WRITE(pins.pin_cs, LOW);
    GPIO_WRITE(pins.pin_rs, HIGH);
    SPI_Write(dat);
    GPIO_WRITE(pins.pin_cs, HIGH);
}

void SPISW_LCD_dat0_n(uint32_t size) {
    // Enter data mode: RS=HIGH at rising edge of 1st SCLK
    GPIO_WRITE(pins.pin_cs, LOW);
    GPIO_WRITE(pins.pin_rs, HIGH);
#if 0
    while (size-- > 0) {
        SPI_Write(0);
    }
#else
#define DTC_UNIT_SIZE0  (0x100)
    while (size > 0) {
        uint32_t count = DTC_UNIT_SIZE0;
        if (size < DTC_UNIT_SIZE0) {
            count = size;
        }
        SPI_TRANSFER(0, 8, (uint8_t *)0, (uint8_t *)0, count, 1000);
        size -= DTC_UNIT_SIZE0;
    }
#endif
    GPIO_WRITE(pins.pin_cs, HIGH);
}

void SPISW_LCD_buf8_1(uint8_t *buf, uint32_t size) {
    // Enter data mode: RS=HIGH at rising edge of 1st SCLK
    GPIO_WRITE(pins.pin_cs, LOW);
    GPIO_WRITE(pins.pin_rs, HIGH);
#if 0
    while (size-- > 0) {
        SPI_Write(*buf++);
    }
#else
#define DTC_UNIT_SIZE   (0x8000)
    while (size > 0) {
        uint32_t count = DTC_UNIT_SIZE;
        if (size < DTC_UNIT_SIZE) {
            count = size;
        }
        SPI_TRANSFER(0, 8, (uint8_t *)0, (uint8_t *)buf, count, 1000);
        size -= count;
    }
#endif
    GPIO_WRITE(pins.pin_cs, HIGH);
}

void SPI_cmd_data_1(uint8_t cmd, uint8_t *data, uint32_t data_size) {
    GPIO_WRITE(pins.pin_cs, LOW);
    GPIO_WRITE(pins.pin_rs, LOW);
    SPI_Write(cmd);
    GPIO_WRITE(pins.pin_rs, HIGH);
    while (data_size-- > 0) {
        SPI_Write(*data++);
    }
    GPIO_WRITE(pins.pin_cs, HIGH);
}

/* ********************************************************************* */
/* LCD Controller: PCF8833                                               */
/* LCD: Nokia 6100                                                       */
/* ********************************************************************* */

void lcdspi_spi_init(lcdspi_t *lcdspi) {
    if (lcdspi->spi_ch != SW_LCDSPI_CH) {
        SPIHW_SetPinMode();
        GPIO_SET_OUTPUT(pins.pin_cs);
        GPIO_SET_OUTPUT(pins.pin_reset);
        GPIO_SET_OUTPUT(pins.pin_rs);
        // ToDo
        SPI_INIT(lcdspi->spi_ch,
                lcdspi->lcdspi_pins->pin_dout,
                lcdspi->lcdspi_pins->pin_din,
                lcdspi->lcdspi_pins->pin_clk,
                lcdspi->lcdspi_pins->pin_cs,
                lcdspi->baud,
                8,
                (lcdspi->phase << 1 | lcdspi->polarity) & 0x3);
        SPI_GET_CONF(lcdspi->spi_ch, &lcdspi->spcmd, &lcdspi->spbr);
    } else {
        SPISW_SetPinMode();
        SPISW_Initialize();
    }
}

static void lcdspi_spi_start_xfer(lcdspi_t *lcdspi) {
    if (lcdspi->spi_ch != SW_LCDSPI_CH) {
        SPI_START_XFER(lcdspi->spi_ch, lcdspi->spcmd, lcdspi->spbr);
    } else {
        SPISW_SetPinMode();
    }
}

static void lcdspi_spi_end_xfer(lcdspi_t *lcdspi) {
    if (lcdspi->spi_ch != SW_LCDSPI_CH) {
        SPI_END_XFER(lcdspi->spi_ch);
    } else {
        SPIHW_SetPinMode();
    }
}

/* ********************************************************************* */
/* LCD Controller: PCF8833                                               */
/* LCD: Nokia 6100                                                       */
/* ********************************************************************* */

static void PCF8833_Reset() {
    delay_ms(100);
    GPIO_WRITE(pins.pin_reset, LOW);
    delay_ms(1000);
    GPIO_WRITE(pins.pin_reset, HIGH);
    delay_ms(100);
}

#define DLYMS   (0x00)
#define CMD8_0  (0x10)
#define DAT8_0  (0x20)
#define CMD8_1  (0x30)
#define DAT8_1  (0x40)

static const uint8_t lcdcmd_PFC8833[] = {
    CMD8_0 + 0, PCF8833_SLEEPOUT,
    CMD8_0 + 0, PCF8833_BSTRON,
    DLYMS, 200,
    CMD8_0 + 1, PCF8833_COLMOD, 0x03,
    CMD8_0 + 1, PCF8833_MADCTL, 0x00,
    CMD8_0 + 1, PCF8833_SETCON, 0x35,
    DLYMS, 200,
    CMD8_0 + 0, PCF8833_DISPON
};
#define PFC8833_CMD_SIZE    (sizeof(lcdcmd_PFC8833)/sizeof(uint8_t))

static const uint8_t lcdcmd_S1D15G10[] = {
    DLYMS, 200,
    CMD8_0 + 3, S1D15G10_DISCTL, 0x0d, 0x20, 0x00,
    CMD8_0 + 1, S1D15G10_COMSCN, 0x01,
    CMD8_0 + 0, S1D15G10_OSCON,
    CMD8_0 + 0, S1D15G10_SLPOUT,
    CMD8_0 + 1, S1D15G10_PWRCTR, 0x0f,
    CMD8_0 + 0, S1D15G10_DISNOR,
    CMD8_0 + 0, S1D15G10_DISINV,
    CMD8_0 + 0, S1D15G10_PTLOUT,
    CMD8_0 + 3, S1D15G10_DATCTL, 0x00, 0x00, 0x02,
    CMD8_0 + 2, S1D15G10_VOLCTR, 0x1c, 0x00,
    CMD8_0 + 1, S1D15G10_TMPGRD, 0x00,
    DLYMS, 100,
    CMD8_0 + 0, S1D15G10_DISON,
};
#define S1D15G10_CMD_SIZE   (sizeof(lcdcmd_S1D15G10)/sizeof(uint8_t))

static const uint8_t lcdcmd_ST7735[] = {
    CMD8_0 + 0, 0x11,
    DLYMS, 50,
    CMD8_1 + 3, 0xb1, 0x01, 0x2c, 0x2d,
    CMD8_1 + 3, 0xb2, 0x01, 0x2c, 0x2d,
    CMD8_1 + 6, 0xb3, 0x01, 0x2c, 0x2d, 0x01, 0x2c, 0x2d,
    CMD8_1 + 1, 0xb4, 0x07, /* column inversion */
    CMD8_1 + 3, 0xc0, 0xa2, 0x02, 0xb4,
    CMD8_1 + 1, 0xc1, 0xc5,
    CMD8_1 + 2, 0xc2, 0x0a, 0x00,
    CMD8_1 + 2, 0xc3, 0x8a, 0x2a,
    CMD8_1 + 2, 0xc4, 0x8a, 0xee,
    CMD8_1 + 1, 0xc5, 0x0e, /* vcom */
    CMD8_1 + 1, 0x36, 0xc8, /* mx, my, rgb mode */
    CMD8_1 + 16, 0xe0, 0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d, 0x29, 0x25, 0x2b, 0x39, 0x00, 0x01, 0x03, 0x10,
    CMD8_1 + 16, 0xe1, 0x03, 0x1d, 0x07, 0x06, 0x2e, 0x2c, 0x29, 0x2d, 0x2e, 0x2e, 0x37, 0x3f, 0x00, 0x00, 0x02, 0x10,
    CMD8_1 + 4, 0x2a, 0x00, 0x02, 0x00, 0x81,
    CMD8_1 + 4, 0x2b, 0x00, 0x01, 0x00, 0xa0,
    CMD8_1 + 1, 0x3a, 0x05, /* 4k mode */
    CMD8_1 + 0, 0x29,   /* display on */
};
#define ST7735_CMD_SIZE   (sizeof(lcdcmd_ST7735)/sizeof(uint8_t))

static const uint8_t lcdcmd_ILI9341[] = {
    CMD8_1 + 0, 0x01,
    DLYMS, 100,
    CMD8_1 + 0, 0x28,
    CMD8_1 + 5, 0xcb, 0x39, 0x2c, 0x00, 0x34, 0x02,
    CMD8_1 + 3, 0xcf, 0x00, 0xc1, 0x30, /* power control b */
    CMD8_1 + 3, 0xe8, 0x85, 0x00, 0x78, /* driver timing control a */
    CMD8_1 + 2, 0xea, 0x00, 0x00, /* driver timing control b */
    CMD8_1 + 4, 0xed, 0x64, 0x03, 0x12, 0x81, /* power on sequence control */
    CMD8_1 + 1, 0xc0, 0x23, /* power control 1 */
    CMD8_1 + 1, 0xc1, 0x10, /* power control 2 */
    CMD8_1 + 2, 0xc5, 0x3e, 0x28, /* vcom control 1 */
    CMD8_1 + 1, 0xc7, 0x86, /* vcom control 2 */
    CMD8_1 + 1, 0x36, 0x48, /* madctl */
    CMD8_1 + 1, 0x3a, 0x55, /* pixel format 16bit */
    CMD8_1 + 2, 0xb1, 0x00, 0x18, /* set frame control */
    CMD8_1 + 3, 0xb6, 0x08, 0x82, 0x27, /* display function control */
    CMD8_1 + 1, 0xf2, 0x02, /* enable 3g: false */
    CMD8_1 + 1, 0x26, 0x01, /* gamma set */
    /* positive gamma correction */
    CMD8_1 + 15, 0xe0, 0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00,
    /* negative gamma correction */
    CMD8_1 + 15, 0xe1, 0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f,
    CMD8_1 + 0, 0x11, /* sleep out */
    DLYMS, 120,
    CMD8_1 + 0, 0x29, /* display on */
};
#define ILI9341_CMD_SIZE   (sizeof(lcdcmd_ILI9341)/sizeof(uint8_t))

static const uint8_t lcdcmd_ILI9340[] = {
    CMD8_1 + 5, 0xcb, 0x39, 0x2c, 0x00, 0x34, 0x02, /* power control a */
    CMD8_1 + 3, 0xcf, 0x00, 0xc1, 0x30, /* power control b */
    CMD8_1 + 3, 0xe8, 0x85, 0x00, 0x78, /* driver timing control a */
    CMD8_1 + 2, 0xea, 0x00, 0x00, /* driver timing control b */
    CMD8_1 + 4, 0xed, 0x64, 0x03, 0x12, 0x81, /* power on squence control */
    CMD8_1 + 1, 0xf7, 0x20, /* pump ratio control */
    CMD8_1 + 1, 0xc0, 0x23, /* power control 1 */
    CMD8_1 + 1, 0xc1, 0x10, /* power control 2 */
    CMD8_1 + 2, 0xc5, 0x3e, 0x28, /* vcom control 1 */
    CMD8_1 + 1, 0xc7, 0x86, /* vcom control 2 */
    CMD8_1 + 1, 0x36, 0x48, /* madctl */
    CMD8_1 + 1, 0x3a, 0x55, /* pixel format */
    CMD8_1 + 2, 0xb1, 0x00, 0x18, /* set frame control */
    CMD8_1 + 3, 0xb6, 0x08, 0x82, 0x27, /* display function control */
    CMD8_1 + 1, 0xf2, 0x00, /* enable 3g */
    CMD8_1 + 1, 0x26, 0x01, /* gamma set */
    CMD8_1 + 15, 0xe0, 0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00,
    CMD8_1 + 15, 0xe1, 0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f,
    CMD8_1 + 0, 0x11, /* sleep out */
    DLYMS, 120,
    CMD8_1 + 0, 0x2c, /* display on */
};
#define ILI9340_CMD_SIZE   (sizeof(lcdcmd_ILI9340)/sizeof(uint8_t))

static void lcdspi_cmd_exec(uint8_t *cmdtbl, uint32_t size) {
    uint8_t typ;
    uint8_t num;
    uint8_t cmd;
    uint32_t i = 0;
    while (i < size) {
        typ = cmdtbl[i] & 0xf0;
        num = cmdtbl[i] & 0x0f;
        cmd = cmdtbl[i+1];
        switch (typ) {
        case DLYMS:
            delay_ms(cmd);
            break;
        case CMD8_0:
            SPISW_LCD_cmd8_0(cmd);
            for (uint32_t j = 0; j < num; j++) {
                SPISW_LCD_dat8_0(cmdtbl[i+2+j]);
            }
            break;
        case CMD8_1:
            SPISW_LCD_cmd8_1(cmd);
            for (uint32_t j = 0; j < num; j++) {
                SPISW_LCD_dat8_1(cmdtbl[i+2+j]);
            }
            break;
        }
        i += (2 + (uint32_t)num);
    }
}

static void PCF8833_Initialize() {
    //SPI_Initialize();
    PCF8833_Reset();
    GPIO_WRITE(pins.pin_cs, LOW);
    GPIO_WRITE(pins.pin_dout, LOW);
    GPIO_WRITE(pins.pin_clk, HIGH);

    lcdspi_cmd_exec((uint8_t *)&lcdcmd_PFC8833, PFC8833_CMD_SIZE);
}

/* ********************************************************************* */
/* LCD Controller: S1D15G10                                              */
/* LCD: Nokia 6100                                                       */
/* ********************************************************************* */

static void S1D15G10_Reset() {
    delay_ms(100);
    GPIO_WRITE(pins.pin_reset, LOW);
    delay_ms(1000);
    GPIO_WRITE(pins.pin_reset, HIGH);
    delay_ms(100);
}

static void S1D15G10_Initialize() {
    //SPI_Initialize();
    S1D15G10_Reset();
    GPIO_WRITE(pins.pin_cs, LOW);
    GPIO_WRITE(pins.pin_dout, LOW);
    GPIO_WRITE(pins.pin_clk, HIGH);

    lcdspi_cmd_exec((uint8_t *)&lcdcmd_S1D15G10, S1D15G10_CMD_SIZE);
}

/* ********************************************************************* */
/* LCD Controller: ST7735                                                */
/* ********************************************************************* */

static void ST7735_Reset() {
    GPIO_WRITE(pins.pin_reset, HIGH);
    delay_ms(100);
    GPIO_WRITE(pins.pin_reset, LOW);
    delay_ms(400);
    GPIO_WRITE(pins.pin_reset, HIGH);
    delay_ms(100);
}

static void ST7735_Initialize() {
    //SPI_Initialize();
    ST7735_Reset();
    GPIO_WRITE(pins.pin_dout, LOW);
    GPIO_WRITE(pins.pin_cs, HIGH);
    GPIO_WRITE(pins.pin_clk, HIGH);

    lcdspi_cmd_exec((uint8_t *)&lcdcmd_ST7735, ST7735_CMD_SIZE);
}

/* ********************************************************************* */
/* LCD Controller: ILI9341                                               */
/* LCD: xxxxxxxxxx                                                       */
/* ********************************************************************* */

static void ILI9341_Reset() {
    GPIO_WRITE(pins.pin_reset, HIGH);
    delay_ms(100);
    GPIO_WRITE(pins.pin_reset, LOW);
    delay_ms(400);
    GPIO_WRITE(pins.pin_reset, HIGH);
    delay_ms(100);
}

static void ILI9341_Initialize() {
    //SPI_Initialize();
    ILI9341_Reset();

    lcdspi_cmd_exec((uint8_t *)&lcdcmd_ILI9341, ILI9341_CMD_SIZE);
}

/* ********************************************************************* */
/* LCD Controller: ILI9340                                               */
/* LCD: xxxxxxxxxx                                                       */
/* ********************************************************************* */

static void ILI9340_Reset() {
    delay_ms(100);
    GPIO_WRITE(pins.pin_reset, LOW);
    delay_ms(400);
    GPIO_WRITE(pins.pin_reset, HIGH);
    delay_ms(100);
}

static void ILI9340_Initialize() {
    //SPI_Initialize();
    ILI9340_Reset();

    lcdspi_cmd_exec((uint8_t *)&lcdcmd_ILI9340, ILI9340_CMD_SIZE);
}

/* ********************************************************************* */
/* LCD Controller: ST7789                                                */
/* ********************************************************************* */

static void ST7789_Reset() {
    GPIO_WRITE(pins.pin_cs, LOW);
    GPIO_WRITE(pins.pin_reset, HIGH);
    delay_ms(50);
    GPIO_WRITE(pins.pin_reset, LOW);
    delay_ms(50);
    GPIO_WRITE(pins.pin_reset, HIGH);
    delay_ms(150);
    GPIO_WRITE(pins.pin_cs, HIGH);
}

static void ST7789_Initialize() {
    ST7789_Reset();

#if 1
    /* soft reset */
    SPISW_LCD_cmd8_1(0x01);
    delay_ms(50);
    /* sleep out */
    SPISW_LCD_cmd8_1(0x11);
    /* color mode */
    SPISW_LCD_cmd8_1(0x3a);
    SPISW_LCD_dat8_1(ST7789_COLORMODE_65K | ST7789_COLORMODE_16BIT);
    delay_ms(50);
    /* MADCTL */
    SPISW_LCD_cmd8_1(0x36);
    SPISW_LCD_dat8_1(0x10);
    /* inversion mode on */
    SPISW_LCD_cmd8_1(0x21);
    delay_ms(10);
    /* norm on */
    SPISW_LCD_cmd8_1(0x13);
    delay_ms(10);
    /* disp on */
    SPISW_LCD_cmd8_1(0x29);
    delay_ms(500);
#else
    /* soft reset */
    SPI_cmd_data_1(0x01, &data, 0);
    delay_ms(150);
    /* sleep out */
    SPI_cmd_data_1(0x11, &data, 0);
    /* color mode */
    data = (ST7789_COLORMODE_65K | ST7789_COLORMODE_16BIT);
    SPI_cmd_data_1(0x3a, &data, 1);
    delay_ms(50);
    /* MADCTL */
    data = 0x10;
    SPI_cmd_data_1(0x36, &data, 1);
    /* inversion mode on */
    SPISW_LCD_cmd8_1(0x21);
    delay_ms(10);
    /* norm on */
    SPISW_LCD_cmd8_1(0x13);
    delay_ms(10);
    /* disp on */
    SPISW_LCD_cmd8_1(0x29);
    delay_ms(1000);
#endif
}

/*
 * common functions
 */

static void ILI9340_addrset(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
#if 0
    uint8_t data[4];
    data[0] = (uint8_t)(x1 >> 8);
    data[1] = (uint8_t)(x1 & 0xff);
    data[2] = (uint8_t)(x2 >> 8);
    data[3] = (uint8_t)(x2 & 0xff);
    SPI_cmd_data_1(ILI9340_CASET, &data[0], 4);
    data[0] = (uint8_t)(y1 >> 8);
    data[1] = (uint8_t)(y1 & 0xff);
    data[2] = (uint8_t)(y2 >> 8);
    data[3] = (uint8_t)(y2 & 0xff);
    SPI_cmd_data_1(ILI9340_PASET, &data[0], 4);
    SPI_cmd_data_1(ILI9340_RAMWR, &data[0], 0);
#else
    SPISW_LCD_cmd8_1(ILI9340_CASET);
    SPISW_LCD_dat8_1((uint8_t)(x1 >> 8));
    SPISW_LCD_dat8_1((uint8_t)(x1 & 0xff));
    SPISW_LCD_dat8_1((uint8_t)(x2 >> 8));
    SPISW_LCD_dat8_1((uint8_t)(x2 & 0xff));
    SPISW_LCD_cmd8_1(ILI9340_PASET);
    SPISW_LCD_dat8_1((uint8_t)(y1 >> 8));
    SPISW_LCD_dat8_1((uint8_t)(y1 & 0xff));
    SPISW_LCD_dat8_1((uint8_t)(y2 >> 8));
    SPISW_LCD_dat8_1((uint8_t)(y2 & 0xff));
    SPISW_LCD_cmd8_1(ILI9340_RAMWR);
#endif
}

void lcdspi_clear(lcdspi_t *lcdspi) {
    uint32_t x;
    uint8_t PASET = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->PASET;
    uint8_t CASET = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->CASET;
    uint8_t RAMWR = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->RAMWR;
    uint32_t PWX = lcdspi->lcdspi_lcd->PWX;
    uint32_t PWY = lcdspi->lcdspi_lcd->PWY;
    uint32_t lcd_ctrl_id = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->id;
    lcdspi_spi_start_xfer(lcdspi);
    if (lcd_ctrl_id == PCF8833 || lcd_ctrl_id == S1D15G10) {
        SPISW_LCD_cmd8_0(PASET);
        SPISW_LCD_dat8_0(0);
        SPISW_LCD_dat8_0((uint8_t)(PWX - 1));
        SPISW_LCD_cmd8_0(CASET);
        SPISW_LCD_dat8_0(0);
        SPISW_LCD_dat8_0((uint8_t)(PWY - 1));
        SPISW_LCD_cmd8_0(RAMWR);
#if 1
        for (x = 0; x < ((PWX * PWY) / 2); x++) {
            SPISW_LCD_dat8_0(0);
            SPISW_LCD_dat8_0(0);
            SPISW_LCD_dat8_0(0);
        }
#else
#endif
    } else {
        //if (lcd_ctrl_id == ILI9340 || lcd_ctrl_id == ST7735 || lcd_ctrl_id == ST7789) {
        ILI9340_addrset((uint16_t)0, (uint16_t)0, (uint16_t)(PWX - 1), (uint16_t)(PWY - 1));
#if 0
        for (y = 0; y < PWY; y++) {
            for (x = 0; x < PWX; x++) {
                SPISW_LCD_dat8_1(0);
                SPISW_LCD_dat8_1(0);
            }
        }
#else
        SPISW_LCD_dat0_n(PWY * PWX *2);
#endif
    }
    lcdspi_spi_end_xfer(lcdspi);
    lcdspi->lcdspi_screen->cx = 0;
    lcdspi->lcdspi_screen->cy = 0;
    return;
}

void lcdspi_screen_init(lcdspi_screen_t *lcdspi_screen) {
    lcdspi_screen->cx = 0;
    lcdspi_screen->cy = 0;
    lcdspi_screen->fcol = (uint16_t)0xFFFFFF;
    lcdspi_screen->bcol = (uint16_t)0x000000;
    lcdspi_screen->unit_wx = 4;
    lcdspi_screen->unit_wy = 8;
}

void lcdspi_init(lcdspi_t *lcdspi) {
    if (lcdspi == NULL) {
        lcdspi = &m_lcdspi;
    }
    g_spi_ch = lcdspi->spi_ch;
    lcdspi_spi_init(lcdspi);
    lcdspi_screen_init(lcdspi->lcdspi_screen);
    //lcdspi_spi_start_xfer(lcdspi);
    lcdspi->lcdspi_lcd->lcdspi_init();
    //lcdspi_spi_end_xfer(lcdspi);
    lcdspi_clear(lcdspi);
}

void lcdspi_deinit(lcdspi_t *lcdspi) {
    if (lcdspi->spi_ch != SW_LCDSPI_CH) {
        // ToDo
        //delete pspi;
    }
}

void lcdspi_bitbltex565(lcdspi_t *lcdspi, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t *data) {
    uint8_t PASET = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->PASET;  // @suppress("Type cannot be resolved")
    uint8_t CASET = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->CASET;
    uint8_t RAMWR = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->RAMWR;
    uint32_t lcd_ctrl_id = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->id;
    uint32_t i, j;
    uint16_t v1, v2;
    uint16_t *pdata = (uint16_t *)data;
    //SPISW_LCD_cmd(DATCTL);  // The DATCTL command selects the display mode (8-bit or 12-bit).
    lcdspi_spi_start_xfer(lcdspi);
    if (lcd_ctrl_id == PCF8833 || lcd_ctrl_id == S1D15G10) {
        for (j = 0; j < height; j++) {
            SPISW_LCD_cmd8_0(PASET);
            SPISW_LCD_dat8_0((uint8_t)(y + j));
            SPISW_LCD_dat8_0((uint8_t)(y + j + 1));
            for (i = 0; i < width; i += 2) {
                SPISW_LCD_cmd8_0(CASET);
                SPISW_LCD_dat8_0((uint8_t)(x + i));
                SPISW_LCD_dat8_0((uint8_t)(x + i + 1));
                v1 = *pdata++;
                v2 = *pdata++;
                SPISW_LCD_cmd8_0(RAMWR);
                SPISW_LCD_dat8_0(R4G4(v1));
                SPISW_LCD_dat8_0(B4R4(v1, v2));
                SPISW_LCD_dat8_0(G4B4(v2));
            }
        }
    } else {
        // if (lcd_ctrl_id == ILI9340 || lcd_ctrl_id == ST7735 || lcd_ctrl_id == ST7789) {
#if 0
        for (j = 0; j < height; j++) {
            ILI9340_addrset((uint16_t)x, (uint16_t)(y + j), (uint16_t)(x + width), (uint16_t)(y + j + 1));
            for (i = 0; i < width; i += 1) {
                v1 = *pdata++;
                SPISW_LCD_dat8_1((uint8_t)v1);
                SPISW_LCD_dat8_1((uint8_t)(v1 >> 8));
            }
        }
#else
        ILI9340_addrset((uint16_t)x, (uint16_t)y, (uint16_t)(x + width - 1), (uint16_t)(y + height - 1));
        SPISW_LCD_buf8_1((uint8_t *)pdata, width * height * 2);
#endif
    }
    lcdspi_spi_end_xfer(lcdspi);
}

void lcdspi_bitbltex(lcdspi_t *lcdspi, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t *data) {
    uint32_t PWX = lcdspi->lcdspi_lcd->PWX;
    uint16_t *pdata = (uint16_t *)data;
    pdata += (y * PWX + x);
    lcdspi_bitbltex565(lcdspi, x, y, width, height, pdata);
}

void lcdspi_write_char_color(lcdspi_t *lcdspi, unsigned char c, uint32_t cx, uint32_t cy, uint16_t fgcol, uint16_t bgcol) {
    uint32_t x, y;
    uint32_t ux, uy;
    uint32_t wx, wy;
    uint16_t col0, col1;
    uint8_t PASET = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->PASET;
    uint8_t CASET = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->CASET;
    uint8_t RAMWR = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->RAMWR;
    uint32_t lcd_ctrl_id = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->id;
    uint32_t text_sx = lcdspi->lcdspi_lcd->text_sx;
    uint32_t text_sy = lcdspi->lcdspi_lcd->text_sy;
    uint32_t disp_wx = lcdspi->lcdspi_lcd->disp_wx;
    uint32_t disp_wy = lcdspi->lcdspi_lcd->disp_wy;
    font_t *font = lcdspi->lcdspi_screen->font;
    uint8_t *data;

    if (font == (font_t *)NULL) {
        return;
    }
    if (c >= 0x80)
        c = 0;
    data = (unsigned char *)font_fontData(font, (int)(c & 0x00ff));
    ux = (uint32_t)font_fontUnitX(font);
    uy = (uint32_t)font_fontUnitY(font);
    wx = (uint32_t)font_fontWidth(font, (int)(c & 0x00ff));
    wy = (uint32_t)font_fontHeight(font, (int)(c & 0x00ff));
    lcdspi_spi_start_xfer(lcdspi);
    if (lcd_ctrl_id == PCF8833 || lcd_ctrl_id == S1D15G10) {
        for (y = 0; y < wy; y++) {
            SPISW_LCD_cmd8_0(CASET);
            SPISW_LCD_dat8_0((uint8_t)(cx * ux + text_sx));
            SPISW_LCD_dat8_0((uint8_t)(disp_wx - 1));
            SPISW_LCD_cmd8_0(PASET);  //y set
            SPISW_LCD_dat8_0((uint8_t)(cy * uy + y + text_sy));
            SPISW_LCD_dat8_0((uint8_t)(disp_wy - 1));
            SPISW_LCD_cmd8_0(RAMWR);
            for (x = 0; x < (wx / 2); x++) {
                if (data[y] & (0x80 >> (x * 2))) {
                    col0 = fgcol;
                } else {
                    col0 = bgcol;
                }
                if (data[y] & (0x40 >> (x * 2))) {
                    col1 = fgcol;
                } else {
                    col1 = bgcol;
                }
                SPISW_LCD_dat8_0((0xff & (uint8_t)(col0 >> 4)));
                SPISW_LCD_dat8_0((0xf0 & (uint8_t)(col0 << 4)) | (0x0f & ((uint8_t)(col1 >> 8))));
                SPISW_LCD_dat8_0((uint8_t)(0xff & col1));
            }
        }
    } else {
        //if (lcd_ctrl_id == ILI9340 || lcd_ctrl_id == ST7735 || lcd_ctrl_id == ST7789) {
        for (y = 0; y < wy; y++) {
            ILI9340_addrset((uint16_t)(cx * ux + text_sx),
                            (uint16_t)(cy * uy + y + text_sy),
                            (uint16_t)(disp_wx - 1),
                            (uint16_t)(disp_wy - 1));
            for (x = 0; x < wx; x++) {
                if (data[y] & (0x80 >> x)) {
                    col0 = fgcol;
                } else {
                    col0 = bgcol;
                }
                SPISW_LCD_dat8_1((uint8_t)(col0 >> 8));
                SPISW_LCD_dat8_1((uint8_t)col0);
            }
        }
    }
    lcdspi_spi_end_xfer(lcdspi);
}

void lcdspi_write_unicode_color(lcdspi_t *lcdspi, unsigned short u, uint32_t cx, uint32_t cy, uint16_t fgcol, uint16_t bgcol) {
    uint32_t x, y;
    uint32_t ux, uy;
    uint32_t wx, wy;
    uint32_t off;
    uint16_t col0, col1;
    uint8_t PASET = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->PASET;
    uint8_t CASET = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->CASET;
    uint8_t RAMWR = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->RAMWR;
    uint32_t lcd_ctrl_id = lcdspi->lcdspi_lcd->lcdspi_ctrl_info->id;
    uint32_t text_sx = lcdspi->lcdspi_lcd->text_sx;
    uint32_t text_sy = lcdspi->lcdspi_lcd->text_sy;
    uint32_t disp_wx = lcdspi->lcdspi_lcd->disp_wx;
    uint32_t disp_wy = lcdspi->lcdspi_lcd->disp_wy;
    font_t *font = lcdspi->lcdspi_screen->font;
    uint8_t *data;

    if (font == (font_t *)NULL) {
        return;
    }
    data = (unsigned char *)font_fontData(font, (int)u);
    ux = (uint32_t)font_fontUnitX(font);
    uy = (uint32_t)font_fontUnitY(font);
    wx = (uint32_t)font_fontWidth(font, (int)u);
    wy = (uint32_t)font_fontHeight(font, (int)u);
    lcdspi_spi_start_xfer(lcdspi);
    off = 0;
    if (lcd_ctrl_id == PCF8833 || lcd_ctrl_id == S1D15G10) {
        for (y = 0; y < wy; y++) {
            SPISW_LCD_cmd8_0(CASET);
            SPISW_LCD_dat8_0((uint8_t)(cx * ux + text_sx));
            SPISW_LCD_dat8_0((uint8_t)(disp_wx - 1));
            SPISW_LCD_cmd8_0(PASET);  //y set
            SPISW_LCD_dat8_0((uint8_t)(cy * uy + y + text_sy));
            SPISW_LCD_dat8_0((uint8_t)(disp_wy - 1));
            SPISW_LCD_cmd8_0(RAMWR);
            for (x = 0; x < wx; x += 2) {
                if (x == 8) {
                    off++;
                }
                if (data[off] & (0x80 >> (x & 0x7))) {
                    col0 = fgcol;
                } else {
                    col0 = bgcol;
                }
                if (data[off] & (0x40 >> (x & 0x7))) {
                    col1 = fgcol;
                } else {
                    col1 = bgcol;
                }
                SPISW_LCD_dat8_0((0xff & (uint8_t)(col0 >> 4)));
                SPISW_LCD_dat8_0((0xf0 & (uint8_t)(col0 << 4)) | (0x0f & ((uint8_t)(col1 >> 8))));
                SPISW_LCD_dat8_0((uint8_t)(0xff & col1));
            }
            off++;
        }
    } else {
        //if (lcd_ctrl_id == ILI9340 || lcd_ctrl_id == ST7735 || lcd_ctrl_id == ST7735) {
        for (y = 0; y < wy; y++) {
            ILI9340_addrset((uint16_t)(cx * ux + text_sx),
                            (uint16_t)(cy * uy + y + text_sy),
                            (uint16_t)(disp_wx - 1),
                            (uint16_t)(disp_wy - 1));
            for (x = 0; x < wx; x++) {
                if (x == 8) {
                    off++;
                }
                if (data[off] & (0x80 >> (x & 0x7))) {
                    col0 = fgcol;
                } else {
                    col0 = bgcol;
                }
                SPISW_LCD_dat8_1((uint8_t)(col0 >> 8));
                SPISW_LCD_dat8_1((uint8_t)col0);
            }
            off++;
        }
    }
    lcdspi_spi_end_xfer(lcdspi);
}

void lcdspi_write_char(lcdspi_t *lcdspi, unsigned char c, uint32_t row, uint32_t col) {
    lcdspi_write_char_color(lcdspi, c, row, col, lcdspi->lcdspi_screen->fcol, lcdspi->lcdspi_screen->bcol);
}

void lcdspi_write_unicode(lcdspi_t *lcdspi, unsigned short u, uint32_t row, uint32_t col) {
    lcdspi_write_unicode_color(lcdspi, u, row, col, lcdspi->lcdspi_screen->fcol, lcdspi->lcdspi_screen->bcol);
}

void lcdspi_write_formatted_char(lcdspi_t *lcdspi, unsigned char ch) {
    lcdspi_screen_t *screen = lcdspi->lcdspi_screen;
    uint32_t cx = screen->cx;
    uint32_t cy = screen->cy;
    uint32_t unit_x = (uint32_t)font_fontUnitX(screen->font);
    uint32_t unit_y = (uint32_t)font_fontUnitY(screen->font);
    uint32_t disp_wx = lcdspi->lcdspi_lcd->disp_wx;
    uint32_t disp_wy = lcdspi->lcdspi_lcd->disp_wy;
    if (ch == 0xc) {
        lcdspi_clear(lcdspi);
        cx = 0;
        cy = 0;
    } else if (ch == '\n') {
        cy++;
        if (cy == disp_wy / unit_y) {
            cy = 0;
        }
    } else if (ch == '\r') {
        cx = 0;
    } else {
        lcdspi_write_char(lcdspi, ch, cx, cy);
        cx++;
        if (cx == disp_wx / unit_x) {
            cx = 0;
            cy++;
            if (cy == disp_wy / unit_y) {
                cy = 0;
            }
        }
    }
    screen->cx = (uint16_t)cx;
    screen->cy = (uint16_t)cy;
}

void lcdspi_write_formatted_unicode(lcdspi_t *lcdspi, unsigned short u) {
    lcdspi_screen_t *screen = lcdspi->lcdspi_screen;
    uint32_t cx = screen->cx;
    uint32_t cy = screen->cy;
    uint32_t unit_x = (uint32_t)font_fontUnitX(screen->font);
    uint32_t unit_y = (uint32_t)font_fontUnitY(screen->font);
    uint32_t disp_wx = lcdspi->lcdspi_lcd->disp_wx;
    uint32_t disp_wy = lcdspi->lcdspi_lcd->disp_wy;
    if ((char)u == 0xc) {
        lcdspi_clear(lcdspi);
        cx = 0;
        cy = 0;
    } else if ((char)u == '\n') {
        cy++;
        if (cy == disp_wy / unit_y) {
            cy = 0;
        }
    } else if ((char)u == '\r') {
        cx = 0;
    } else {
        lcdspi_write_unicode(lcdspi, u, cx, cy);
        if (u < 0x100) {
            cx++;
        } else {
            cx += 2;
        }
        if (cx >= disp_wx / unit_x) {
            cx = 0;
            cy++;
            if (cy == disp_wy / unit_y) {
                cy = 0;
            }
        }
    }
    screen->cx = (uint16_t)cx;
    screen->cy = (uint16_t)cy;
}

void lcdspi_set_font(lcdspi_t *lcdspi, font_t *font) {
    lcdspi->lcdspi_screen->font = font;
}

font_t *lcdspi_get_font(lcdspi_t *lcdspi) {
    return lcdspi->lcdspi_screen->font;
}

unsigned short cnvUtf8ToUnicode(unsigned char *str, uint32_t *size) {
    unsigned int u = 0;
    unsigned char c = *str++;
    int len = 0;
    if ((c & 0x80) == 0) {
        u = c & 0x7F;
        len = 0;
    } else if ((c & 0xE0) == 0xC0) {
        u = c & 0x1F;
        len = 1;
    } else if ((c & 0xF0) == 0xE0) {
        u = c & 0x0F;
        len = 2;
    } else if ((c & 0xF8) == 0xF0) {
        u = c & 0x07;
        len = 3;
    } else if ((c & 0xFC) == 0xF8) {
        u = c & 0x03;
        len = 4;
    } else if ((c & 0xFE) == 0xFC) {
        u = c & 0x01;
        len = 5;
    }
    *size = (uint32_t)(len + 1);
    while (len-- > 0 && ((c = *str) & 0xC0) == 0x80) {
        u = (u << 6) | (unsigned int)(c & 0x3F);
        str++;
    }
    return (unsigned short)u;
}
