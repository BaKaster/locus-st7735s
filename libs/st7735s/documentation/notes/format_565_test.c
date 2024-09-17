/* MIT License
 * Copyright (c) 2021, Michal Kozakiewicz, github.com/michal037
 *
 * code repository: https://github.com/michal037/driver-ST7735S
 * code version: 3.0.0
 */

#include <stdio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)     \
    ((byte) & 0x80 ? '1' : '-'), \
    ((byte) & 0x40 ? '1' : '-'), \
    ((byte) & 0x20 ? '1' : '-'), \
    ((byte) & 0x10 ? '1' : '-'), \
    ((byte) & 0x08 ? '1' : '-'), \
    ((byte) & 0x04 ? '1' : '-'), \
    ((byte) & 0x02 ? '1' : '-'), \
    ((byte) & 0x01 ? '1' : '-')

void test(unsigned char red, unsigned char green, unsigned char blue) {
    unsigned char lcd_buffer[2];

    lcd_buffer[0] = (red & 0xF8)  |  ((green >> 5) & 0x07);
    lcd_buffer[1] = ((green << 3) & 0xE0)  |  ((blue >> 3) & 0x1F);

    printf("%2X %2X %2X ", red, green, blue);
    printf(BYTE_TO_BINARY_PATTERN " ",  BYTE_TO_BINARY(lcd_buffer[0]));
    printf(BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(lcd_buffer[1]));
}

int main() {
    test(255,   0,   0); /* red */
    test(  0, 255,   0); /* green */
    test(  0,   0, 255); /* blue */
    printf("\n");

    /* red */
    for(int i = 0; i <= 7; i++) {
        test(1 << i, 0, 0);
    }
    printf("\n");

    /* green */
    for(int i = 0; i <= 7; i++) {
        test(0, 1 << i, 0);
    }
    printf("\n");

    /* blue */
    for(int i = 0; i <= 7; i++) {
        test(0, 0, 1 << i);
    }

    return 0;
}

/* output:

FF  0  0 11111--- --------
 0 FF  0 -----111 111-----
 0  0 FF -------- ---11111

 1  0  0 -------- --------
 2  0  0 -------- --------
 4  0  0 -------- --------
 8  0  0 ----1--- --------
10  0  0 ---1---- --------
20  0  0 --1----- --------
40  0  0 -1------ --------
80  0  0 1------- --------

 0  1  0 -------- --------
 0  2  0 -------- --------
 0  4  0 -------- --1-----
 0  8  0 -------- -1------
 0 10  0 -------- 1-------
 0 20  0 -------1 --------
 0 40  0 ------1- --------
 0 80  0 -----1-- --------

 0  0  1 -------- --------
 0  0  2 -------- --------
 0  0  4 -------- --------
 0  0  8 -------- -------1
 0  0 10 -------- ------1-
 0  0 20 -------- -----1--
 0  0 40 -------- ----1---
 0  0 80 -------- ---1----

*/
