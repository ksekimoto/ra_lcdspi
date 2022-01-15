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

#include <stdint.h>
#include <stdbool.h>
#include "hal_data.h"
#include "common.h"
#include "fsp_gpio.h"
#include "fsp_sci_spi.h"

#if defined(USE_SCI_SPI)

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif

#define RESET_VALUE (0)

static volatile spi_event_t g_master_event_flag;  // Master Transfer Event completion flag

void sci_spi_callback(spi_callback_args_t *p_args) {
    if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event) {
        g_master_event_flag = SPI_EVENT_TRANSFER_COMPLETE;
    } else {
        g_master_event_flag = SPI_EVENT_TRANSFER_ABORTED;
    }
}

void fsp_sci_spi_set_bits(uint32_t ch, uint32_t bits) {
    // ToDo: implementation
}

void fsp_sci_spi_set_clk(uint32_t ch, uint32_t baud) {
    sci_spi_div_setting_t sclk_div;
    bool use_mddr = false;
    fsp_err_t err = FSP_SUCCESS;                     // Error status    uint8_t spcr;
    if (baud == 0)
        return;
    err = R_SCI_SPI_CalculateBitrate (baud, &sclk_div, use_mddr);
    if (FSP_SUCCESS != err) {
        //APP_ERR_PRINT("\r\nMaster R_SPI_Write() failed");
        return;
    }
    uint8_t scr = g_spi0_ctrl.p_reg->SCR;
    g_spi0_ctrl.p_reg->SCR = 0;
    g_spi0_ctrl.p_reg->BRR = sclk_div.brr;
    if (use_mddr) {
        g_spi0_ctrl.p_reg->MDDR = sclk_div.mddr;
    }
    uint32_t smr = (uint32_t)g_spi0_ctrl.p_reg->SMR;
    smr &= ~(0x3 << R_SCI0_SMR_CKS_Pos);
    smr |= (uint32_t) (sclk_div.cks << R_SCI0_SMR_CKS_Pos);
    g_spi0_ctrl.p_reg->SMR = (uint8_t)smr;
    g_spi0_ctrl.p_reg->SCR = (uint8_t)scr;
}

void fsp_sci_spi_set_firstbit(uint32_t ch, uint32_t firstbit) {
    // ToDo: implementation
}

void fsp_sci_spi_set_spi_ch(uint32_t ch, uint32_t polarity, uint32_t phase) {
    // ToDo: implementation
}

void fsp_sci_spi_transfer8(uint32_t ch, uint8_t *dst, uint8_t *src, uint32_t count) {
    fsp_err_t err = FSP_SUCCESS;                     // Error status
    g_master_event_flag = (spi_event_t)RESET_VALUE;  // Reseting master_event flag
    uint8_t val;
    err = R_SCI_SPI_WriteRead (&g_spi0_ctrl, (const void *)src, (void *)dst, count, SPI_BIT_WIDTH_8_BITS);
    if (FSP_SUCCESS != err) {
        //APP_ERR_PRINT("\r\nMaster R_SPI_Write() failed");
        return;
    }
    while (SPI_EVENT_TRANSFER_COMPLETE != g_master_event_flag) {
    }
    return;
}

void fsp_sci_spi_transfer(uint32_t ch, uint32_t bits, uint8_t *dst, uint8_t *src, uint32_t count, uint32_t timeout) {
    fsp_err_t err = FSP_SUCCESS;                     // Error status
    g_master_event_flag = (spi_event_t)RESET_VALUE;  // Reseting master_event flag
    uint8_t val;
    err = R_SCI_SPI_WriteRead (&g_spi0_ctrl, (const void *)src, (void *)dst, count, SPI_BIT_WIDTH_8_BITS);
    if (FSP_SUCCESS != err) {
        //APP_ERR_PRINT("\r\nMaster R_SPI_Write() failed");
        return;
    }
    while (SPI_EVENT_TRANSFER_COMPLETE != g_master_event_flag) {
    }
    return;
}

void fsp_sci_spi_start_xfer(uint32_t ch, uint16_t spcmd, uint8_t spbr) {
    // ToDo: implementation
}

void fsp_sci_spi_end_xfer(uint32_t ch) {
    // ToDo: implementation
}

void fsp_sci_spi_get_conf(uint32_t ch, uint16_t *spcmd, uint8_t *spbr) {
    // ToDo: implementation
}

uint8_t fsp_sci_spi_write_byte(uint32_t ch, uint8_t b) {
    fsp_err_t err = FSP_SUCCESS;                     // Error status
    g_master_event_flag = (spi_event_t)RESET_VALUE;  // Reseting master_event flag
    uint8_t val = 0;
    err = R_SCI_SPI_WriteRead (&g_spi0_ctrl, (const void *)&b, (void *)&val, 1, SPI_BIT_WIDTH_8_BITS);
    if (FSP_SUCCESS != err) {
        //APP_ERR_PRINT("\r\nMaster R_SPI_Write() failed");
        return val;
    }
    while (SPI_EVENT_TRANSFER_COMPLETE != g_master_event_flag) {
    }
    return val;
}

void fsp_sci_spi_write_bytes8(uint32_t ch, uint8_t *buf, uint32_t count) {
    fsp_err_t err = FSP_SUCCESS;                     // Error status
    g_master_event_flag = (spi_event_t)RESET_VALUE;  // Reseting master_event flag
    err = R_SCI_SPI_Write(&g_spi0_ctrl, (const void *)buf, count, SPI_BIT_WIDTH_8_BITS);
    if (FSP_SUCCESS != err) {
        //APP_ERR_PRINT("\r\nMaster R_SPI_Write() failed");
        return;
    }
    while (SPI_EVENT_TRANSFER_COMPLETE != g_master_event_flag) {
    }
    return;
}

void fsp_sci_spi_read_bytes8(uint32_t ch, uint8_t *buf, uint32_t count) {
    fsp_err_t err = FSP_SUCCESS;                     // Error status
    g_master_event_flag = (spi_event_t)RESET_VALUE;  // Reseting master_event flag
    err = R_SCI_SPI_Read(&g_spi0_ctrl, (void *)buf, count, SPI_BIT_WIDTH_8_BITS);
    if (FSP_SUCCESS != err) {
        //APP_ERR_PRINT("\r\nMaster R_SPI_Read() failed");
        return;
    }
    while (SPI_EVENT_TRANSFER_COMPLETE != g_master_event_flag) {
    }
    return;
}

void fsp_sci_spi_init(uint32_t ch, uint32_t mosi, uint32_t miso, uint32_t clk, uint32_t cs, uint32_t speed, uint32_t bits, uint32_t mode) {
    fsp_err_t err = FSP_SUCCESS;  // Error status

    fsp_gpio_write(cs, 1);
    /* Open/Initialize SPI Master module */
    err = R_SCI_SPI_Open(&g_spi0_ctrl, &g_spi0_cfg);
    /* handle error */
    if (FSP_SUCCESS != err) {
        /* SPI Master Failure message */
        //APP_ERR_PRINT("** R_SPI_Open API for SPI Master failed ** \r\n");
        return;
    }
}

void fsp_sci_spi_deinit(uint32_t ch) {
    // ToDo: implementation
}

#endif
