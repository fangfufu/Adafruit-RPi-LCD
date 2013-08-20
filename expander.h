#ifndef EXP_H
#define EXP_H

/**
 * @file expander.h
 * @brief functions related to the MCP23017 I/O expander
 */

#include <stdint.h>

/**
 * @brief GPIO Port Enumeration
 */
typedef enum {
    PortA = 0x00,       /**<      Expander Port A         */
    PortB = 0x10        /**<      Expander Port B         */
} Port;

/**
 * @brief GPIO Register Enumeration
 */
typedef enum {
    IODIR = 0x00,       /**<      IO Direction Register            */
    IOPOL = 0x01,       /**<      Input Polarity Register          */
    IOCON = 0x05,       /**<      General Configuration Register   */
    GPPU  = 0x06,       /**<      Pull-up  Register                */
    GPIO  = 0x09        /**<      GPIO Register                    */
} Reg;

/**
 * @brief Initialise the I/O Expander
 * @return
 * - on success: 0
 * - on partial failure: the number of bytes sent to the expander
 * - on failure related to open()/ioctl(): -1
 */
int exp_init();

/**
 * @brief close down the I/O Expander
 * @return
 * - on success: 0
 * - on failure: whatever the close() call returns.
 */
int exp_close();

/**
 * @brief Write a byte to a register in the expander
 * @return
 * - on success: 0
 * - on partial failure: the return of the write() call.
 * - on complete failure: -1
 */
int exp_write(Port port, Reg reg, uint8_t val);

/**
 * @brief Read a single byte from a register to a buffer
 */
uint8_t exp_read(Port port, Reg reg);

#endif