/*
 * xpt2046.c
 *
 *  Created on: 2021/01/27
 *      Author: ksgadget
 */

#include <stdint.h>
#include <stdbool.h>
#include "fsp_spi.h"
#include "xpt2046.h"

#define LV_HOR_RES  240
#define LV_VER_RES  320

static uint8_t xpt2046_spi_xchg(xpt2046_t *xpt, uint8_t dat) {
    uint8_t data_rec = 0;
    fsp_spi_transfer(xpt->ch, 8, (uint8_t *)&data_rec, (uint8_t *)&dat, 1, 1000);
    return data_rec;

}

static void xpt2046_corr(xpt2046_t *xpt, int16_t *x, int16_t *y) {
    if (xpt->xy_swap){
        int16_t swap_tmp;
        swap_tmp = *x;
        *x = *y;
        *y = swap_tmp;
    }
    if ((*x) > xpt->x_min) {
        (*x) -= xpt->x_min;
    } else {
        (*x) = 0;
    }
    if ((*y) > xpt->y_min) {
        (*y) -= xpt->y_min;
    } else {
        (*y) = 0;
    }
    (*x) = (uint32_t)((uint32_t)(*x) * LV_HOR_RES) / (xpt->x_max - xpt->x_min);
    (*y) = (uint32_t)((uint32_t)(*y) * LV_VER_RES) / (xpt->y_max - xpt->y_min);
    if (xpt->x_inv){
        (*x) =  LV_HOR_RES - (*x);
    }
    if (xpt->y_inv){
        (*y) =  LV_VER_RES - (*y);
    }
}

static void xpt2046_avg(xpt2046_t *xpt, int16_t *x, int16_t *y) {
    uint8_t i;
    for(i = XPT2046_AVG - 1; i > 0 ; i--) {
        xpt->avg_buf_x[i] = xpt->avg_buf_x[i - 1];
        xpt->avg_buf_y[i] = xpt->avg_buf_y[i - 1];
    }
    /*Insert the new point*/
    xpt->avg_buf_x[0] = *x;
    xpt->avg_buf_y[0] = *y;
    if (xpt->avg_last < XPT2046_AVG) {
        xpt->avg_last++;
    }
    /*Sum the x and y coordinates*/
    int32_t x_sum = 0;
    int32_t y_sum = 0;
    for (i = 0; i < xpt->avg_last ; i++) {
        x_sum += xpt->avg_buf_x[i];
        y_sum += xpt->avg_buf_y[i];
    }
    /*Normalize the sums*/
    (*x) = (int32_t)x_sum / xpt->avg_last;
    (*y) = (int32_t)y_sum / xpt->avg_last;
}

bool xpt2046_read(xpt2046_t *xpt, uint16_t *data_x, uint16_t *data_y) {
    static int16_t last_x = 0;
    static int16_t last_y = 0;
    bool valid = true;
    uint8_t buf;
    int16_t x = 0;
    int16_t y = 0;

    uint8_t irq = (uint8_t)fsp_gpio_read(xpt->irq);
    if (irq == 0) {
        fsp_gpio_write(xpt->cs, 0);
        xpt2046_spi_xchg(xpt, CMD_X_READ);
        buf = xpt2046_spi_xchg(xpt, 0);
        x = buf << 8;
        buf = xpt2046_spi_xchg(xpt, CMD_Y_READ);
        x += buf;
        buf = xpt2046_spi_xchg(xpt, 0);
        y = buf << 8;
        buf = xpt2046_spi_xchg(xpt, 0);
        y += buf;
        fsp_gpio_write(xpt->cs, 1);

        x = x >> 3;
        y = y >> 3;
        xpt2046_corr(xpt, &x, &y);
        xpt2046_avg(xpt, &x, &y);
        last_x = x;
        last_y = y;
    } else {
        x = last_x;
        y = last_y;
        xpt->avg_last = 0;
        valid = false;
    }
    *data_x = x;
    *data_y = y;
    return valid;
}

void xpt2046_init(xpt2046_t *xpt, uint32_t ch, uint32_t mosi, uint32_t miso, uint32_t sck, uint32_t cs, uint32_t irq, uint32_t baud) {
    xpt->ch = ch;
    xpt->mosi = mosi;
    xpt->miso = miso;
    xpt->sck = sck;
    xpt->cs = cs;
    xpt->irq = irq;
    xpt->baud = baud;
    fsp_gpio_mode_input(irq);
    fsp_gpio_mode_output(cs);
    fsp_gpio_write(cs, 1);
    fsp_spi_init(ch, mosi, miso, sck, cs, baud, 8, 0);
}

void xpt2046_deinit(uint32_t ch) {
    // ToDo: implementation
}
