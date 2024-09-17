/* MIT License
 * Copyright (c) 2021, Michal Kozakiewicz, github.com/michal037
 *
 * code repository: https://github.com/michal037/driver-ST7735S
 * code version: 3.0.0
 */

#include <stdio.h> /* printf */
#include <stdlib.h> /* exit, size_t, malloc, free */
#include <math.h> /* demo */
#include "st7735s.h"

/* only for the example implementation with Wiring Pi */
/* Wiring Pi reference: http://wiringpi.com/reference/ */
#include <wiringPi.h>
#include <wiringPiSPI.h>
/* SPI buffer size of Wiring Pi; http://wiringpi.com/reference/spi-library/ */
#define WPI_SPI_BUFFER_SIZE  4096 /* x * 1024 */
#define WPI_SPI_CHANNEL         0
#define PIN_COMMUNICATION_MODE  9 /* Wiring Pi pin numbering */
#define PIN_RESET               8
#define PIN_CHIP_SELECT        10

/* ######################## HAVE TO BE IMPLEMENTED ########################## */
/* These three functions HAVE TO BE LINKED to the code from the st7735s.c file.
 * They can be implemented in another file than this one.
 */

void lcd_delay(unsigned long int milliseconds) {
    delay(milliseconds);
}

void lcd_digitalWrite(unsigned short int pin, unsigned char value) {
    digitalWrite(pin, value);
}

void lcd_spiWrite(unsigned char* buffer, size_t length) {
    /* ALREADY TESTED INSIDE THE LIBRARY:
     * 1. if(buffer == NULL)
     * 2. if(length <= 0)
     * You do not need to test this.
     */
    wiringPiSPIDataRW(WPI_SPI_CHANNEL, buffer, length);
}
/* ################## END - HAVE TO BE IMPLEMENTED - END #################### */

/* auxiliary functions implemented below */
void fail_handler();
lcd_status_t demo();

int main(void) {
    /* You can use software SPI. I used the hardware one because I had
     * it available. It all depends on how you implement the lcd_spiWrite
     * function and connect the display module to Raspberry Pi.
     * This is an example of communication documented as 4-line SPI.
     *
     * #### STEP 1
     * Download and install the Wiring Pi library.
     * Official page: http://wiringpi.com/download-and-install/
     *
     * #### STEP 2
     * Enable support for the hardware SPI interface using raspi-config.
     * https://www.raspberrypi.com/documentation/computers/configuration.html
     * https://www.raspberrypi.com/documentation/computers/raspberry-pi.html
     *
     * #### STEP 3
     * I connected my display module to the Raspberry Pi using GPIO diagram from
     * the following page: https://pinout.xyz/pinout/spi
     * You can also display the GPIO diagram with the 'gpio readall' command.
     *
     * Module -> Raspberry Pi 0
     *
     * LED ->  1 pin / 3V3 Power
     * SCK -> 23 pin / SPI0 SCLK
     * SDA -> 19 pin / SPI0 MOSI
     * A0  ->  5 pin / WPI_PIN 9
     * RST ->  3 pin / WPI_PIN 8
     * CS  -> 24 pin / SPI0 CE0 / WPI_PIN 10
     * GND -> 25 pin / Ground
     * VCC -> 17 pin / 3V3 Power
     */

    /* Pointer for display module related settings. */
    lcd_ptr_t my_lcd_settings = NULL;

    /* Initialize the Wiring Pi library */
    wiringPiSetup();

    /* Initialize SPI interface; http://wiringpi.com/reference/spi-library/
     *
     * Based on Timing Chart for 4-line SPI. (pdf v1.4 p36)
     *
     * Serial Clock Cycle (Write) is 66 [ns].
     *
     * 1 [s] / 66 [ns] = 1 / 0.000000066 = 15151515.151515152 [Hz]
     *
     * Based on this information, I conclude that the maximum frequency
     * is 15 MHz = 15 000 000 Hz.
     */
    if(wiringPiSPISetup(WPI_SPI_CHANNEL, 15000000) == -1) {
        fail_handler();
    }

    /* PIN_RESET HAVE TO be stable when the display module is powered on.
     * If not, perform a hardware reset before communicating with the display
     * driver. If you do not do this, the driver will not work properly!
     * The software reset is not suitable.
     */

    /* You HAVE TO manually set the correct pins as outputs! */
    pinMode(PIN_COMMUNICATION_MODE, OUTPUT);
    pinMode(PIN_RESET,              OUTPUT);
    pinMode(PIN_CHIP_SELECT,        OUTPUT);

    /* In order to communicate with the display module, you HAVE TO select
     * the correct one using the Chip Select line.
     * When using multiple display modules, you have to manually manage their
     * selection through the Chip Select lines.
     *
     * ST7735S driver is selected with LOW state. (pdf v1.4 p23)
     */

    /* select display module; activation of communication */
    digitalWrite(PIN_CHIP_SELECT, LOW);

    /* create settings for a specific display module */
    my_lcd_settings = lcd_createSettings(
        128, /* width */
        160, /* height */
        0,   /* width_offset */
        0,   /* height_offset */
        PIN_COMMUNICATION_MODE, /* Wiring Pi pin numbering */
        PIN_RESET
    );

    /* lcd_createSettings will return NULL if memory runs out */
    if(my_lcd_settings == NULL) {
        fail_handler();
    }

    /* This is where you set the settings as active for the library.
     * The library will use them to handle the driver.
     * You can swap it at any time for a different display module.
     * If set to NULL, library functions will return LCD_FAIL.
     */
    lcd_setSettingsActive(my_lcd_settings);

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

    /* ready to draw */

    /* demo; HIGH MEMORY AND CPU CONSUMPTION */
    if(demo() < LCD_OK) {
        fail_handler();
    }

    /* deselect display module; deactivation of communication */
    digitalWrite(PIN_CHIP_SELECT, HIGH);

    /* free memory; prevent memory leak */
    lcd_deleteSettings(my_lcd_settings);

    return 0;
}

/* fail checking is optional but very useful */
void fail_handler() {
    printf("[!] FAILURE OCCURRED WHILE PROCESSING [!]\n");
    exit(EXIT_FAILURE);
}

/* DEMO DEMO DEMO DEMO DEMO DEMO DEMO DEMO DEMO DEMO DEMO DEMO DEMO DEMO DEMO */

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} color_t;

lcd_status_t demo() {
    unsigned short int width, height;
    unsigned char pixel_size;
    unsigned char *framebuffer;
    size_t length_framebuffer;
    size_t address;
    color_t palette[256];
    unsigned char formula; /* max 255 for palette */
    unsigned short int x, y;
    size_t i, p;

    /* Prevent a bug of NULL pointer. 'lcd_settings' is required. */
    if(lcd_settings == NULL) {
        return LCD_FAIL;
    }

    /* get width and height */
    width  = lcd_settings->width;
    height = lcd_settings->height;

    /* get pixel size */
    switch(lcd_settings->interface_pixel_format) {
        /* case LCD_PIXEL_FORMAT_444 is not supported for this function */

        case LCD_PIXEL_FORMAT_565:
            pixel_size = 2; /* two bytes required */
            break;

        case LCD_PIXEL_FORMAT_666:
            pixel_size = 3; /* three bytes required */
            break;

        /* unknown interface pixel format */
        default:
            return LCD_FAIL;
    }

    /* allocate memory */
    length_framebuffer = width * height * pixel_size;
    framebuffer = malloc(length_framebuffer);

    /* if out of RAM memory */
    if(framebuffer == NULL) {
        return LCD_FAIL;
    }

    /* set window position */
    if(lcd_setWindowPosition(0, 0, width - 1, height - 1) < LCD_OK) {
        free(framebuffer);
        return LCD_FAIL;
    }

    /* activate memory write */
    if(lcd_activateMemoryWrite() < LCD_OK) {
        free(framebuffer);
        return LCD_FAIL;
    }

    /* draw */
    for(i = 128; i < 1024; i++) {
        /* create palette */
        for(p = 0; p < 256; p++) {
            palette[p].red   = ((i % 512) - 128) / 1.8;
            palette[p].green = 128 + 127 * sin((3.14159 * p /  64.0) + 1);
            palette[p].blue  = 128 + 127 * sin((3.14159 * p / 128.0) + 1);
        }

        /* populate framebuffer */
        for(x = 0; x < width; x++) {
            for(y = 0; y < height; y++) {
                formula = 128 + 127 * sin(
                    3.14159 *
                    (x - width  / (2 + 2 * sin(3.14159 * ((i % 512) - 128) / 384))) *
                    (y - height / (2 + 2 * sin(3.14159 * ((i % 512) - 128) / 384))) /
                    ((i % 512) * 10.0)
                );

                /* calculate pixel address */
                address = (y * width + x) * pixel_size;

                /* write pixel to framebuffer */
                switch(lcd_settings->interface_pixel_format) {
                    /* case LCD_PIXEL_FORMAT_444 is not supported
                     * for this function
                     */

                    case LCD_PIXEL_FORMAT_565:
                        framebuffer[address] = (palette[formula].red & 0xF8) |
                            ((palette[formula].green >> 5) & 0x07);

                        framebuffer[address + 1] =
                            ((palette[formula].green << 3) & 0xE0) |
                            ((palette[formula].blue  >> 3) & 0x1F);
                        break;

                    case LCD_PIXEL_FORMAT_666:
                        framebuffer[address]     = palette[formula].red;
                        framebuffer[address + 1] = palette[formula].green;
                        framebuffer[address + 2] = palette[formula].blue;
                        break;

                    /* unknown interface pixel format */
                    default:
                        free(framebuffer);
                        return LCD_FAIL;
                }
            }
        }

        /* send framebuffer */
        if(
            lcd_framebuffer_send(
                framebuffer,
                length_framebuffer,
                WPI_SPI_BUFFER_SIZE
            ) < LCD_OK
        ) {
            free(framebuffer);
            return LCD_FAIL;
        }
    }

    /* free memory */
    free(framebuffer);

    return LCD_OK;
}
