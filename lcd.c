/**
 * @file lcd.c
 */
#include <stdio.h>

#include "expander.h"

#include "lcd.h"

/* Port B polarity settings */
#define LCD_BUSY        0x02
#define LCD_READY       0x00

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

#ifdef DBG
    printf("Red: %u\nGreen: %u\nBlue: %u\nregA: %x\nregB: %x\n",
           GPIOA_buf.pin.RED,
           GPIOA_buf.pin.GREEN,
           GPIOB_buf.pin.BLUE,
           GPIOA_buf.reg,
           GPIOB_buf.reg
          );
#endif

    if (r == 0) {
        return r;
    }
    printf("Colour change error: %d\n", r);
    return r;
}