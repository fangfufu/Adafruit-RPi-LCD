#include <stdio.h>
#include <unistd.h>
#include "button.h"
#include "debug.h"
#include "gpio.h"
#include "lcd.h"

int main()
{
    exp_init();
    LCD_byte(0x02, 0);
    LCD_byte(0x20, 0);
    LCD_byte(0x0E, 0);
    LCD_byte(0x06 ,0);
    LCD_byte(0x48, 1);
    LCD_byte(0x48, 1);
    LCD_byte(0x48, 1);
    //btn_printf(btn_blk());
    //dbg_colour_check();
}