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

#ifndef FSP_SCI_SPI_H_
#define FSP_SCI_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "hal_data.h"

void sci_spi_callback(spi_callback_args_t *p_args);
void fsp_sci_spi_set_bits(uint32_t ch, uint32_t bits);
void fsp_sci_spi_set_clk(uint32_t ch, uint32_t spi_clk);
void fsp_sci_spi_set_firstbit(uint32_t ch, uint32_t firstbit);
void fsp_sci_spi_set_spi_ch(uint32_t ch, uint32_t polarity, uint32_t phase);
void fsp_sci_spi_transfer8(uint32_t ch, uint8_t *dst, uint8_t *src, uint32_t count);
void fsp_sci_spi_transfer(uint32_t ch, uint32_t bits, uint8_t *dst, uint8_t *src, uint32_t count, uint32_t timeout);
void fsp_sci_spi_start_xfer(uint32_t ch, uint16_t spcmd, uint8_t spbr);
void fsp_sci_spi_end_xfer(uint32_t ch);
void fsp_sci_spi_get_conf(uint32_t ch, uint16_t *spcmd, uint8_t *spbr);
uint8_t fsp_sci_spi_write_byte(uint32_t ch, uint8_t b);
void fsp_sci_spi_write_bytes8(uint32_t ch, uint8_t *buf, uint32_t count);
void fsp_sci_spi_read_bytes8(uint32_t ch, uint8_t *buf, uint32_t count);
void fsp_sci_spi_init(uint32_t ch, uint32_t mosi, uint32_t miso, uint32_t clk, uint32_t cs, uint32_t speed, uint32_t bits, uint32_t mode);

#ifdef __cplusplus
}
#endif

#endif /* FSP_SCI_SPI_H_ */
