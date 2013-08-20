/**
 * @file lcd.c
 * @note For the LEDs on the LCD display, logic value '1' actually turns them
 * off, whereas logic value '0' turns them on.
 */
#include <stdio.h>

#include "expander.h"

#include "lcd.h"

/* Port B polarity settings */
#define LCD_BUSY        0x02
#define LCD_READY       0x00

/* Colour definitions */
#define RED             (1 << 6)
#define GREEN           (1 << 7)
#define BLUE            1

int LCD_colour(Colour colour)
{
    /* strip the colour off all LEDs first */
    uint8_t bufA = exp_read(PortA, GPIO);
    uint8_t bufB = exp_read(PortB, GPIO);
    bufA |= RED | GREEN;
    bufB |= BLUE;

    switch(colour) {
        case Black:
            break;
        case Red:
            bufA &= ~RED;
            break;
        case Yellow:
            bufA &= ~(RED | GREEN);
            break;
        case Green:
            bufA &= ~GREEN;
            break;
        case Cyan:
            bufA &= ~GREEN;
            bufB &= ~BLUE;
            break;
        case Blue:
            bufB &= ~BLUE;
            break;
        case Magenta:
            bufA &= ~RED;
            bufB &= ~BLUE;
            break;
        case White:
            bufA &= ~(RED | GREEN);
            bufB &= ~BLUE;
            break;
    }
    int r = exp_write(PortA, GPIO, bufA);
    r += exp_write(PortB, GPIO, bufB);

    if (r == 0) {
        return r;
    }
    printf("Colour change error: %d\n", r);
    return r;
}