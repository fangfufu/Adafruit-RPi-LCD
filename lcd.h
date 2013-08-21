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
 * @note The data pins for the LCD are connected in the reverse order on the
 * I/O expander. Hence we need to manually map pins.
 */
int LCD_nibble(uint8_t data, int RS);

/**
 * @brief Send a byte to the LCD screen
 * @details This function calls send_nibble() twice.
 * @return
 * - on success: 0
 * - on error: accumulated error from the send_nibble() call.
 */
int LCD_byte(uint8_t data, int RS);

/**
 * @brief change LCD colour
 * @return
 * - on success: 0
 * - on error: the accumulated errors from GPIO_write() calls.
 */
int LCD_colour(Colour colour);
#endif