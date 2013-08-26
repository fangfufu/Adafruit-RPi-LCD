#include <stdint.h>
#include <stdio.h>

#include "lcd_lowlevel.h"
#include "lcd.h"

/** Set Character Generation RAM (CGRAM) address */
#define CGRAM                   0x40

static uint8_t g_DDRAM_addr;

/**
 * @brief save DDRAM address
 * @return the saved DDRAM address
 */
static uint8_t DDRAM_addr_save()
{
    g_DDRAM_addr = LL_addr();
    return g_DDRAM_addr;
}

/**
 * @brief restore DDRAM address
 * @return 0, if successful; -1, if failed
 */
static int DDRAM_addr_restore()
{
    uint8_t old_DDRAM_addr = g_DDRAM_addr;
    LCD_cmd(DDRAM|g_DDRAM_addr);
    if (old_DDRAM_addr == g_DDRAM_addr) {
        return 0;
    }
    printf("LL_DDRAM_addr_restore: error: could not restore DDRAM address");
    return -1;
}