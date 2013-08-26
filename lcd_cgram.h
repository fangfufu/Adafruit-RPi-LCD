#ifndef CGRAM_H
#define CGRAM_H

/**
 * @brief write a custom character to CGRAM
 * @param[in] n the custom character number
 * @param[in] data an uint8_t array with 8 elements containing the custom
 * character
 * @return 0 on success
 */
int CGRAM_write_character(int n, uint8_t data[8]);

/**
 * @brief read a custom character from the CGRAM
 * @param[in] n the custom character number
 * @param[in] data an uint8_t array with 8 elements for storing the custom
 * character
 * @return 0 on success

 */
int CGRAM_read_character(int n, uint8_t data[8]);

#endif