//
// hagl_hal.h
//
// Created on: Jan 24, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

// Porting code for HAGL graphics library

#ifndef CORE_INC_HAGL_HAL_H
#define CORE_INC_HAGL_HAL_H

#include <stdint.h>
#include "lcd.h"

#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT ST7735S_LCD_HEIGHT
#endif // DISPLAY_HEIGHT

#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH ST7735S_LCD_WIDTH
#endif // DISPLAY_WIDTH

#ifndef DISPLAY_DEPTH
#define DISPLAY_DEPTH (24)
#endif // DISPLAY_DEPTH

#define HAGL_HAS_HAL_COLOR

#ifndef HAGL_HAS_HAL_FILL_RECTANGLE
#define HAGL_HAS_HAL_FILL_RECTANGLE
#endif // HAGL_HAS_HAL_FILL_RECTANGLE

#ifndef HAGL_HAS_HAL_CLEAR_SCREEN
#define HAGL_HAS_HAL_CLEAR_SCREEN
#endif // HAGL_HAS_HAL_CLEAR_SCREEN

#ifndef HAGL_HAS_HAL_CLEAR_SCREEN_COLOR
#define HAGL_HAS_HAL_CLEAR_SCREEN_COLOR
#endif // HAGL_HAS_HAL_CLEAR_SCREEN_COLOR

typedef uint32_t color_t;

#if defined(__cplusplus)
extern "C" {
#endif

void hagl_hal_put_pixel(uint16_t x0, uint16_t y0, color_t);
color_t hagl_hal_color(unsigned char r, unsigned char g, unsigned char b);
void hagl_hal_fill_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color);
void hagl_hal_clear_screen();
void hagl_hal_clear_screen_color(color_t);

#if defined(__cplusplus)
}
#endif

#endif // CORE_INC_HAGL_HAL_H
