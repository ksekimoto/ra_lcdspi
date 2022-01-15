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
#if defined(_WIN32)
#include <string.h>
#include <windows.h>
#else
#include "common.h"
#include "ff.h"
#endif
#include "lcdspi.h"
#include "jpeg_disp.h"
#include "lcdspi_sample.h"

#if defined(_WIN32)
#else
static FATFS fatfs;
#endif

static bool g_split_disp = true;
static lcdspi_lcd_t g_lcdspi_lcd = {
    .disp_wx = 240,
    .disp_wy = 320,
    .PWX = 240,
    .PWY = 320,
    .text_sx = 0,
    .text_sy = 0
};
static lcdspi_screen_t g_lcdspi_screen = {
    (font_t *)NULL,
    0,
    0,
    0xffff,
    0x0000,
    12,
    12
};
static lcdspi_t g_lcdspi = {
    .lcdspi_lcd = &g_lcdspi_lcd,
    .lcdspi_screen = &g_lcdspi_screen,
};


#if defined(_WIN32)


#define FPATH_MAX 256
char root[FPATH_MAX];

void lcdspi_disp_jpg_files(lcdspi_t* lcdspi, uint32_t x, uint32_t y, const char* dir, bool split_disp)
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;
    FILETIME ft;
    SYSTEMTIME st;
    char path[FPATH_MAX];
    uint32_t path_len;

    strcpy((char *)&root, (const char *)dir);
    strcat((char *)&root, (const char *)"\\*.jpg");
    hFind = FindFirstFileA((LPCSTR)&root, &fd);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("File not found.\n");
        return;
    }
    do {
        FileTimeToSystemTime(&ft, &st);
        printf("file name: %s - ", fd.cFileName);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            printf("(dir)");
        }
        printf("file size: %d\n", fd.nFileSizeLow);
        lcdspi_clear(lcdspi);
        path_len = (uint32_t)(strlen((const char *)dir) + 1 + strlen((const char *)fd.cFileName));
        if (path_len < FPATH_MAX) {
            strcpy((char*)&path, (const char*)dir);
            strcat((char*)&path, (const char*)"\\");
            strcat((char*)&path, (const char*)fd.cFileName);
            jpeg_disp_file(lcdspi, x, y, (const char*)&path, split_disp);
        }
    } while (FindNextFileA(hFind, &fd));
    FindClose(hFind);
}

#else

static bool fs_init(void);
static bool fs_init(void) {
    FRESULT res = f_mount((FATFS *)&fatfs, "", 1);
    return (res == FR_OK)? true:false;
}

//FRESULT f_findfirst (
//  DIR* dp,              /* [OUT] Poninter to the directory object */
//  FILINFO* fno,         /* [OUT] Pointer to the file information structure */
//  const TCHAR* path,    /* [IN] Pointer to the directory name to be opened */
//  const TCHAR* pattern  /* [IN] Pointer to the matching pattern string */
//);

void lcdspi_disp_jpg_files(lcdspi_t *lcdspi, uint32_t x, uint32_t y, const char *dir, bool split_disp) {
    FRESULT fr;     /* Return value */
    DIR dj;         /* Directory object */
    FILINFO fno;    /* File information */

    fr = f_findfirst(&dj, &fno, dir, "*.jp*");
    while (fr == FR_OK && fno.fname[0]) {         /* Repeat while an item is found */
        //printf("%s\n", fno.fname);
        lcdspi_init(lcdspi);
        jpeg_disp_file(lcdspi, x, y, (const char *)fno.fname, split_disp);
        mdelay(2000);
        fr = f_findnext(&dj, &fno);               /* Search for next item */
    }
    f_closedir(&dj);
}

#endif

void lcdspi_sample(void) {
#if defined(_WIN32)
    lcdspi_disp_jpg_files((lcdspi_t*)&g_lcdspi, 50, 50, (const char*)".\\data", g_split_disp);
#else
#if defined(HW_LCDSPI_RS)
    fsp_gpio_mode_output(HW_LCDSPI_RS);
    fsp_gpio_write(HW_LCDSPI_RS, 1);
#endif
    if (!fs_init()) {
        return;
    }
    lcdspi_set_spi_ch((lcdspi_t *)&g_lcdspi, 0);
    lcdspi_set_pins((lcdspi_t *)&g_lcdspi, (lcdspi_pins_t *)NULL);
    lcdspi_set_lcd((lcdspi_t *)&g_lcdspi, 0xffffffff);
    lcdspi_set_screen((lcdspi_t *)&g_lcdspi, &g_lcdspi_screen);
    lcdspi_disp_jpg_files((lcdspi_t*)&g_lcdspi, 0, 0, (const char*)"\\", g_split_disp);
#endif
}
