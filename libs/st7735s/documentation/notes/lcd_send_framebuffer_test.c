/* MIT License
 * Copyright (c) 2021, Michal Kozakiewicz, github.com/michal037
 *
 * code repository: https://github.com/michal037/driver-ST7735S
 * code version: 3.0.0
 */

#include <stdio.h>

typedef enum lcd_status_t {
    LCD_FAIL = -1,
    LCD_OK   =  0
} lcd_status_t;

lcd_status_t lcd_writeData(unsigned char* buffer, size_t length) {
    printf("address: %p, amount: %d\n", buffer, (unsigned int)length);
    return LCD_OK;
}

lcd_status_t lcd_framebuffer_send(
    unsigned char * buffer,
    const    size_t length_buffer,
    const    size_t length_chunk
) {
    unsigned int i;
    unsigned int chunk_amount;
    unsigned int remainder;

    /* check inputs */
    if(buffer == NULL) {
        return LCD_FAIL;
    }
    if(length_buffer <= 0) {
        return LCD_FAIL;
    }
    if(length_chunk <= 0) {
        return LCD_FAIL;
    }

    chunk_amount = length_buffer / length_chunk; /* integer division */
    remainder    = length_buffer % length_chunk;

    /* send chunks */
    for(i = 0; i < chunk_amount; i++) {
        if(
            lcd_writeData(
                buffer + i * length_chunk,
                length_chunk
            ) < LCD_OK
        ) {
            return LCD_FAIL;
        }
    }

    /* send the remainder, if any */
    if(remainder > 0) {
        if(
            lcd_writeData(
                buffer + chunk_amount * length_chunk,
                remainder
            ) < LCD_OK
        ) {
            return LCD_FAIL;
        }
    }

    return LCD_OK;
}

int main() {
    const size_t length_chunk = 4;
    unsigned char *ptr = (unsigned char*)0x10;
    size_t i;

    for(i = 0; i <= 10; i++) {
        printf("length_buffer: %lu\n", i);
        lcd_framebuffer_send(ptr, i, length_chunk);
        printf("\n");
    }

    return 0;
}

/* output:

length_buffer: 0

length_buffer: 1
address: 0x10, amount: 1

length_buffer: 2
address: 0x10, amount: 2

length_buffer: 3
address: 0x10, amount: 3

length_buffer: 4
address: 0x10, amount: 4

length_buffer: 5
address: 0x10, amount: 4
address: 0x14, amount: 1

length_buffer: 6
address: 0x10, amount: 4
address: 0x14, amount: 2

length_buffer: 7
address: 0x10, amount: 4
address: 0x14, amount: 3

length_buffer: 8
address: 0x10, amount: 4
address: 0x14, amount: 4

length_buffer: 9
address: 0x10, amount: 4
address: 0x14, amount: 4
address: 0x18, amount: 1

length_buffer: 10
address: 0x10, amount: 4
address: 0x14, amount: 4
address: 0x18, amount: 2

*/
