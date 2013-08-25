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
    while(1){
        getchar();
        LCD_home();
        LCD_putchar('A');
    }
}