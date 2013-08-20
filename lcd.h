#ifndef LCD_H
#define LCD_H

/**
 * @file lcd.h
 * @brief Functions related to the LCD screen
 */

/**
 * @brief The colours available on the LCD
 */
typedef enum {
    Black,
    Red,
    Yellow,
    Green,
    Cyan,
    Blue,
    Magenta,
    White
} Colour;

/**
 * @brief change LCD colour
 * @return
 * - on success: 0
 * - on error: the accumulated errors from GPIO_write() calls.
 */
int LCD_colour(Colour colour);
#endif