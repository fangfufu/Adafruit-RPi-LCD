#ifndef DBG_H
#define DBG_H
/**
 * @file debug.h
 * @brief common functions useful for debugging
 */

/**
 * @brief Print the status of GPIO ports
 */
void dbg_print_ports();

/**
 * @brief cycle through all colours
 */
void dbg_colour_check();

/**
 * @brief print out the status of all LEDs
 */
void dbg_led_status();

/**
 * @brief print a button clock
 */
void dbg_print_clk();

/**
 * @brief print RW_TEST_LENGTH characters to the LCD
 */
void LCD_write_test();

/**
 * @brief read RW_TEST_LENGTH characters back from LCD
 * @details RW_TEST_LENGTH is defined in debug.c
 */
void LCD_read_test();

#endif
