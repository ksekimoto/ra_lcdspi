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
#include "common.h"
#if defined(USE_SEGGER_RTT)
#include "SEGGER_RTT.h"
#endif
#include "fsp_sci.h"

void user_uart_callback(uart_callback_args_t *p_args);

static volatile bool uart_recv_flag = false;
static volatile bool uart_send_complete_flag = false;

/* Callback function */
void user_uart_callback(uart_callback_args_t *p_args) {
    /* TODO: add your own code here */
    if (p_args->event == UART_EVENT_RX_COMPLETE) {
        uart_recv_flag = true;
    } else if (p_args->event == UART_EVENT_TX_COMPLETE) {
        uart_send_complete_flag = true;
    }
}

#if defined(USE_SEGGER_RTT)

uint8_t fsp_sci_rx_ch() {
    return (uint8_t)SEGGER_RTT_GetKey();
}

void fsp_sci_tx_ch(uint8_t c) {
    SEGGER_RTT_Write(0, (const void *)&c, (unsigned)1);
}

void fsp_sci_init(void) {
    SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal\r\n");
    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}

void fsp_sci_deinit(void) {

}

#else
uint8_t fsp_sci_rx_ch() {
    uint8_t c;
    fsp_err_t status = FSP_SUCCESS;
    status = R_SCI_UART_Read(&g_uart0_ctrl, &c, 1);
    if (FSP_SUCCESS != status) {
        __BKPT();
    }
    while (!uart_recv_flag) {
    }
    uart_recv_flag = false;
    return c;
}

void fsp_sci_tx_ch(uint8_t c) {
    fsp_err_t status = FSP_SUCCESS;
    status = R_SCI_UART_Write(&g_uart0_ctrl, &c, 1);
    if (FSP_SUCCESS != status) {
        __BKPT();
    }
    while (!uart_send_complete_flag) {
    }
    uart_send_complete_flag = false;
}

void fsp_sci_init(void) {
    fsp_err_t status = FSP_SUCCESS;
    status = R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    if (FSP_SUCCESS != status) {
        __BKPT();
    }
}

void fsp_sci_deinit(void) {
    fsp_err_t status = FSP_SUCCESS;
    status = R_SCI_UART_Close(&g_uart0_ctrl);
    if (FSP_SUCCESS != status) {
        __BKPT();
    }
}

#endif
