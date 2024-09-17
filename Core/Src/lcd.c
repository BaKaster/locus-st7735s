//
// lcd.c
//
// Created: 2022-01-18
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <geoscan.aero>)
//

#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_spi.h"
#include "stm32f051x8.h"
#include "lcd.h"
#include "gpio.h"
#include <st7735s.h>

// Numeric constants related to St7735
enum {
	St7735PinCsNum = GPIO_PIN_4,
	St7735PinDcNum = GPIO_PIN_6,
	St7735PinResetNum = -1,
	St7735SpiTimeout = HAL_MAX_DELAY,
};

// ------------------------------ vars. ------------------------------

GPIO_TypeDef *const kSt7735PinCsPort = GPIOA;
GPIO_TypeDef *const kSt7735PinDcPort = GPIOA;
GPIO_TypeDef *const kSt7735PinResetPort = GPIOA;
extern SPI_HandleTypeDef hspi1;
static SPI_HandleTypeDef *const kSt7735SpiHandle = &hspi1;
static lcd_ptr_t sLcdSettings = NULL;

// ------------------------------ private functions ------------------------------

static void fail_handler();

// ------------------------------ public functions ------------------------------

// Separate API implementations for st7735c's driver

void lcd_delay(unsigned long int aMilliseconds)
{
	HAL_Delay(aMilliseconds);
}

void lcd_digitalWrite(unsigned short int aPin, unsigned char aValue)
{
	if (aPin < 0) {
		return;
	}

	switch ((St7735Pin)aPin) {
		case St7735PinCs:
			HAL_GPIO_WritePin(kSt7735PinCsPort, St7735PinCsNum, aValue);
			break;

		case St7735PinDc:
			HAL_GPIO_WritePin(kSt7735PinDcPort, St7735PinDcNum, aValue);
			break;

		case St7735PinReset:
			HAL_GPIO_WritePin(kSt7735PinResetPort, St7735PinResetNum, aValue);
			break;

		default:
			break;
	};
}

void lcd_spiWrite(unsigned char* aBuffer, size_t aLength)
{
	HAL_GPIO_WritePin(kSt7735PinCsPort, St7735PinCsNum, 0);
	HAL_SPI_Transmit(kSt7735SpiHandle, aBuffer, aLength, St7735SpiTimeout);
	HAL_GPIO_WritePin(kSt7735PinCsPort, St7735PinCsNum, 1);
}

// Other functions

///
/// @brief Initialize those GPIO that were not initialized by the generated
/// peripherals initialization code, namely "DC" and "CS".
///
/// @note As for 2022-01-19, "RST" pin is not used
///
void lcdGpioInit()
{
	GPIO_InitTypeDef gpioInit = {0};

	gpioInit.Pin = St7735PinDcNum;
	gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInit.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(kSt7735PinDcPort, &gpioInit);

	gpioInit.Pin = St7735PinCsNum;
	HAL_GPIO_Init(kSt7735PinCsPort, &gpioInit);
}

///
/// \brief lcdInit Default SW initialization code. Subset of what is offered by
/// the ST7735 driver (see main_example_wpi.c), with very slight changes mostly
/// comprised of removing the irrelevant HW-related part.
///
void lcdSwInit()
{
	if (NULL != sLcdSettings) {
		return;
	}

	/* create settings for a specific display module */
	sLcdSettings = lcd_createSettings(
		ST7735S_LCD_WIDTH, /* width */
		ST7735S_LCD_HEIGHT, /* height */
		0,   /* width_offset */
		0,   /* height_offset */
		St7735PinDc, /* Wiring Pi pin numbering */
		St7735PinReset
	);

	/* lcd_createSettings will return NULL if memory runs out */
	if(sLcdSettings == NULL) {
		fail_handler();
	}

	/* This is where you set the settings as active for the library.
	 * The library will use them to handle the driver.
	 * You can swap it at any time for a different display module.
	 * If set to NULL, library functions will return LCD_FAIL.
	 */
	lcd_setSettingsActive(sLcdSettings);

	/* Display initialization. It HAVE TO be done for each display separately.
	 * The library will do this for the appropriate display module,
	 * based on the active settings.
	 */
	if(lcd_initialize() < LCD_OK) {
		fail_handler();
	}

	/* To start drawing, you HAVE TO:
	 * step 1: turn off sleep mode
	 * step 2: set Memory Access Control  - required by lcd_createSettings()
	 * step 3: set Interface Pixel Format - required by lcd_createSettings()
	 * step 4: turn on the display
	 * After that, you can draw.
	 *
	 * It is best to make the optional settings between steps 1 and 4.
	 */

	/* turn off sleep mode; required to draw */
	if(lcd_setSleepMode(LCD_SLEEP_OUT) < LCD_OK) {
		fail_handler();
	}

	/* set Memory Access Control; refresh - required by lcd_createSettings() */
	if(lcd_setMemoryAccessControl(LCD_MADCTL_DEFAULT) < LCD_OK) {
		fail_handler();
	}

	/* set Interface Pixel Format; refresh - required by lcd_createSettings() */
	if(lcd_setInterfacePixelFormat(LCD_PIXEL_FORMAT_666) < LCD_OK) {
		fail_handler();
	}

	/* set Predefined Gamma; optional reset */
	if(lcd_setGammaPredefined(LCD_GAMMA_PREDEFINED_3) < LCD_OK) {
		fail_handler();
	}

	/* set Display Inversion; optional reset */
	if(lcd_setDisplayInversion(LCD_INVERSION_OFF) < LCD_OK) {
		fail_handler();
	}

	/* set Tearing Effect Line; optional reset */
	if(lcd_setTearingEffectLine(LCD_TEARING_OFF) < LCD_OK) {
		fail_handler();
	}

	/* turn on the display; required to draw */
	if(lcd_setDisplayMode(LCD_DISPLAY_ON) < LCD_OK) {
		fail_handler();
	}
}

void fail_handler()
{
}
