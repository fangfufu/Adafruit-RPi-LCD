#include <stdio.h>
#include <unistd.h>
#include "button.h"
#include "gpio.h"
#include "lcd.h"
#include "lcd_cgram.h"

int main()
{
    GPIO_open();
    LCD_init();
}