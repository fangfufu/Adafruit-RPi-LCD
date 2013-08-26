#include <stdio.h>
#include <unistd.h>
#include "button.h"
#include "debug.h"
#include "gpio.h"
#include "lcd.h"

void test()
{

}

int main()
{
    GPIO_open();
    LCD_init();

    printf("LCD_cursor_goto: %d\n", LCD_cursor_goto(0, 16));

    while(1){
        getchar();
        LCD_putchar('A');
    }
}