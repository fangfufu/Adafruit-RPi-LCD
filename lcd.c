#include <stdio.h>

#include "gpio.h"

#include "lcd.h"

typedef union {
    uint8_t byte;
    struct {
        unsigned int _0  : 1;
        unsigned int _1  : 1;
        unsigned int _2  : 1;
        unsigned int _3  : 1;
        unsigned int _4  : 1;
        unsigned int _5  : 1;
        unsigned int _6  : 1;
        unsigned int _7  : 1;
    } bit;
} Byte;

int LCD_nibble(uint8_t data, int RS)
{
    Byte b;
    b.byte = data;
    /* Control pins */
    GPIOB_buf.pin.RS = RS;
    GPIOB_buf.pin.R = 0;
    GPIOB_buf.pin.E = 1;
    /* Data pins */
    GPIOB_buf.pin.DB7 = b.bit._3;
    GPIOB_buf.pin.DB6 = b.bit._2;
    GPIOB_buf.pin.DB5 = b.bit._1;
    GPIOB_buf.pin.DB4 = b.bit._0;
    /* The actual sending sequence */
    int r = GPIO_write(PortB);
    GPIOB_buf.pin.E = 0;
    r += GPIO_write(PortB);
    return r;
}

int LCD_byte(uint8_t data, int RS)
{
    int r = LCD_nibble (data >> 4, RS);
    r += LCD_nibble(data, RS);
    return r;
}

/**
 * @note For the LEDs on the LCD display, logic value '1' actually turns them
 * off, whereas logic value '0' turns them on.
 */
int LCD_colour(Colour colour)
{
    switch(colour) {
        case Black:
            GPIOA_buf.pin.RED = 1;
            GPIOA_buf.pin.GREEN = 1;
            GPIOB_buf.pin.BLUE = 1;
            break;
        case Red:
            GPIOA_buf.pin.RED = 0;
            GPIOA_buf.pin.GREEN = 1;
            GPIOB_buf.pin.BLUE = 1;
            break;
        case Yellow:
            GPIOA_buf.pin.RED = 0;
            GPIOA_buf.pin.GREEN = 0;
            GPIOB_buf.pin.BLUE = 1;
            break;
        case Green:
            GPIOA_buf.pin.RED = 1;
            GPIOA_buf.pin.GREEN = 0;
            GPIOB_buf.pin.BLUE = 1;
            break;
        case Cyan:
            GPIOA_buf.pin.RED = 1;
            GPIOA_buf.pin.GREEN = 0;
            GPIOB_buf.pin.BLUE = 0;
            break;
        case Blue:
            GPIOA_buf.pin.RED = 1;
            GPIOA_buf.pin.GREEN = 1;
            GPIOB_buf.pin.BLUE = 0;
            break;
        case Magenta:
            GPIOA_buf.pin.RED = 0;
            GPIOA_buf.pin.GREEN = 1;
            GPIOB_buf.pin.BLUE = 0;
            break;
        case White:
            GPIOA_buf.pin.RED = 0;
            GPIOA_buf.pin.GREEN = 0;
            GPIOB_buf.pin.BLUE = 0;
            break;
    }
    int r = GPIO_write(PortA);
    r += GPIO_write(PortB);

    if (r == 0) {
        return r;
    }
    printf("Colour change error: %d\n", r);
    return r;
}