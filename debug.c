#include <stdio.h>
#include <unistd.h>

#include "expander.h"
#include "lcd.h"

void dbg_print_ports()
{
    printf("Port A %x\tPort B: %x\n",
           exp_read(PortA, GPIO),
           exp_read(PortB, GPIO)
          );
}

void dbg_colour_check()
{
    LCD_colour(Black);
    printf("Black\n");
    sleep(1);
    LCD_colour(Red);
    printf("Red\n");
    sleep(1);
    LCD_colour(Yellow);
    printf("Yellow\n");
    sleep(1);
    LCD_colour(Green);
    printf("Green\n");
    sleep(1);
    LCD_colour(Cyan);
    printf("Cyan\n");
    sleep(1);
    LCD_colour(Blue);
    printf("Blue\n");
    sleep(1);
    LCD_colour(Magenta);
    printf("Magenta\n");
    sleep(1);
    LCD_colour(White);
    printf("White\n");
    sleep(1);
}