//
// hagl_hal.c
//
// Created on: Jan 24, 2022
//     Author: Dmitry Murashov (dmtrDOTmurashovATgmailDOTcom (gmail.com))
//

#include "hagl_hal.h"
#include "st7735s.h"

void decodeColor(color_t aColor, unsigned char &r, unsigned char &g, unsigned char &b)
{
	r = (aColor >> 16) & 0xFF;
	g = (aColor >> 8) & 0xFF;
	b = aColor & 0xFF;
}

extern "C" void hagl_hal_put_pixel(uint16_t aX, uint16_t aY, color_t aColor)
{
	unsigned char r, g, b;
	decodeColor(aColor, r, g, b);
	lcd_drawPixel(aX, aY, r, g, b);
}

extern "C" color_t hagl_hal_color(unsigned char r, unsigned char g, unsigned char b)
{
	color_t color = 0;

	color |= (r << 16);
	color |= (g << 8);
	color |= b;

	return color;
}

extern "C" void hagl_hal_fill_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color)
{
	unsigned char r, g, b;
	decodeColor(color, r, g, b);
	lcd_drawFilledRectangle(x0, y0, x1, y1, r, g, b);
}

void hagl_hal_clear_screen()
{
	lcd_clearScreen(255, 255, 255);
}

void hagl_hal_clear_screen_color(color_t color)
{
	unsigned char r, g, b;
	decodeColor(color, r, g, b);
	lcd_clearScreen(r, g, b);
}
