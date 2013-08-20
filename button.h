#ifndef BUTTON_H
#define BUTTON_H
/**
 * @file button.h
 * @brief functions related to the GPIO buttons
 */

#include <stdint.h>

/**
 * @brief The buttons on the panel
 */
typedef enum {
    Select = 1,
    Right = 2,
    Down = 4,
    Up = 8,
    Left = 16,
    Null
} Button;

/**
 * @brief Read button pressing (non-blocking version)
 */
Button btn_nblk();

/**
 * @brief Read raw button data (non-blocking version)
 */
uint8_t btn_nblk_raw();

/**
 * @brief Read button pressing (blocking version)
 */
Button btn_blk();

/**
 * @brief Read raw button data (blocking version)
 */
uint8_t btn_blk_raw();

/**
 * @brief Print a button press to the stdout
 */
int btn_printf(Button button);

#endif