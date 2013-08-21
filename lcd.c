/**
 * @file lcd.c
 * @note The LCD instruction are defined in pg. 24 of the data sheet.
 */
#include <stdio.h>

#include "gpio.h"

#include "lcd.h"

/** Clear display                       */
#define CLEAR                   0x01

/** Return home                         */
#define HOME                    0x02

/** Entry mode set                      */
#define ENTRY_SET               0x04
#define INCREMENT               0x02
#define DECREMENT               0x00
#define SHIFT_DISPLAY           0x01

/** Display on/off control              */
#define DISPLAY_SET             0x08
#define DISPLAY_ON              0x04
#define CURSOR_ON               0x02
#define CURSOR_BLINK_ON         0x01

/** Cursor or display shift             */
#define SHIFT                   0x10
#define DISPLAY                 0x08
#define CURSOR                  0x00
#define RIGHT                   0x04
#define LEFT                    0x00

/**
 * @brief Function set
 * @details pg.24 of the datasheet
 * - Interface length = 4 bits,
 * - Display lines = 2,
 * - Character font = 5 x 10 dots (seems to make no difference)
 */
#define FUNCTION_SET            0x2C

/** Special initialisation code for 4-bit mode */
#define FOUR_BIT_MODE           0x02

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

/**
 * @brief Primitive LCD send command
 * @details This function does two things:
 * - send command down the LCD,
 * - switch the E pin.
 * @param[in] data please put the data in the lower nibble.
 * @param[in] RS
 * - 0: Select instruction register
 * - 1: Select data register
 * @return
 * - on success: 0
 * - on error: accumulated error from the GPIO_write() call.
 * @note
 * - The data pins for the LCD are connected in the reverse order on the
 * I/O expander. Hence we need to manually map pins.
 * - Removed from the header file because calling this function randomly might
 * cause misalignment in the command sequence, so you can't send stuff to the
 * LCD again once you exit the program. Please call LCD_byte() instead, and
 * fill the first nibble with 0s.
 */
static int LCD_nibble(uint8_t data, int RS)
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
    /* The actual sending sequence. Note that data is clocked in at the
    lowering edge of the E pin. */
    int r;
    r = GPIO_write(PortB);
    GPIOB_buf.pin.E = 0;
    r += GPIO_write(PortB);
    return r;
}

/**
 * @brief Send a byte to the LCD screen
 * @details This function calls send_nibble() twice.
 * @return
 * - on success: 0
 * - on error: accumulated error from the send_nibble() call.
 */
static int LCD_byte(uint8_t data, int RS)
{
    int r;
    r = LCD_nibble (data >> 4, RS);
    r += LCD_nibble(data, RS);
    return r;
}

int LCD_cmd(uint8_t cmd)
{
    return LCD_byte(cmd, 0);
}

int LCD_data(uint8_t data)
{
    return LCD_byte(data, 1);
}

int LCD_init()
{
    int r;
    r  = LCD_cmd(FOUR_BIT_MODE);
    r += LCD_cmd(FUNCTION_SET);
    r += LCD_cmd(DISPLAY_SET | DISPLAY_ON | CURSOR_ON | CURSOR_BLINK_ON);
    r += LCD_cmd(ENTRY_SET|INCREMENT);
    r += LCD_cmd(CLEAR);
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
    int r;
    r = GPIO_write(PortA);
    r += GPIO_write(PortB);

    if (r == 0) {
        return r;
    }
    printf("Colour change error: %d\n", r);
    return r;
}