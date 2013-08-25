#include <stdio.h>
#include <unistd.h>
#include "button.h"
#include "debug.h"
#include "gpio.h"
#include "lcd.h"

void test()
{
    LCD_write_test();
    LCD_read_test();
}

int main()
{
    GPIO_open();
    LCD_init();
    while(1){
        getchar();
        LCD_putchar('A');
        getchar();
        printf("%c", read_byte(1));
    }
}