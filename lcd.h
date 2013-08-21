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
 * @brief Initialise the LCD screen
 */
int LCD_init();

/**
 * @brief change LCD colour
 * @return
 * - on success: 0
 */
int LCD_colour(Colour colour);

/**
 * @brief Send a command to the LCD screen.
 */
int LCD_cmd(uint8_t cmd);

/**
 * @brief Send a data byte to the LCD screen.
 */
int LCD_data(uint8_t data);

#endif