/**
 * @brief restore DDRAM address
 * @return
 * - 0, if the address counter is now point to DDRAM.
 * - -1.
 */

int LL_DDRAM_addr_restore()
{
    uint8_t old_DDRAM_addr = g_DDRAM_addr;
    LCD_cmd(DDRAM|g_DDRAM_addr);
    if (old_DDRAM_addr == g_DDRAM_addr) {
        return 0;
    }
    printf("LL_DDRAM_addr_restore: error: could not restore DDRAM address")
    return -1;
}