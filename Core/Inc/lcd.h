//
// lcd.h
//
// Created: 2022-01-19
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> geoscan.aero)
//

#if !defined(CODE_INC_LCD_H)
#define CODE_INC_LCD_H

#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define ST7735S_LCD_WIDTH (128)
#define ST7735S_LCD_HEIGHT (160)

typedef enum {
	St7735PinNotUsed = -1,

	// Shortcut names for pins
	St7735PinCs = 0,
	St7735PinDc,  // Data/command, or communication mode
	St7735PinReset,
} St7735Pin;

void lcdGpioInit();
void lcdSwInit();
int lcdExample();
void lcd_spiWrite(unsigned char* aBuffer, size_t aLength);
void lcd_digitalWrite(unsigned short int aPin, unsigned char aValue);
void lcd_delay(unsigned long int aMilliseconds);

#if defined(__cplusplus)
}
#endif

#endif // CODE_INC_LCD_H

