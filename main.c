#include <stdio.h>
#include <unistd.h>
#include "button.h"
#include "debug.h"
#include "gpio.h"
#include "lcd.h"

#define LCD_LENTH               16;
#define SECOND_LINE_START       40;

int main()
{
    exp_init();
    LCD_init();
    int i = 0;
    while(1){
        getchar();
        LCD_data(i + '0');
        printf("%d\n", i);
        i++;
    }
}