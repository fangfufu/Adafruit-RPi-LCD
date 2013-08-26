#include <stdio.h>
#include <unistd.h>
#include "button.h"
#include "debug.h"
#include "gpio.h"
#include "lcd.h"
#include "lcd_cgram.h"

int main()
{
    GPIO_open();
    LCD_init();

    uint8_t data[8];
    data[0] = 0xE;
    data[1] = 0x1B;
    data[2] = 0x11;
    data[3] = 0x11;
    data[4] = 0x11;
    data[5] = 0x11;
    data[6] = 0x11;
    data[7] = 0x1F;

    uint8_t output[8];
    CGRAM_write_character(0, data);
    CGRAM_read_character(0, output);

    for (int i = 0; i < 8; i++){
        printf("%x\n", output[i]);
    }

    while(1){
        getchar();
        LCD_putchar(0);
    }
}