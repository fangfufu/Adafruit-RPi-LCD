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

#endif
