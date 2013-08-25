#include <stdio.h>
#include <unistd.h>

#include "gpio.h"
#include "lcd.h"
#include "button.h"

#define RW_TEST_LENGTH  80

void dbg_print_ports()
{
    printf("Port A %x\tPort B: %x\n",
           GPIO_read(PortA),
           GPIO_read(PortB)
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

void dbg_led_status()
{
    printf("Red: %u\nGreen: %u\nBlue: %u\nregA: %x\nregB: %x\n",
           GPIOA_buf.pin.RED,
           GPIOA_buf.pin.GREEN,
           GPIOB_buf.pin.BLUE,
           GPIOA_buf.reg,
           GPIOB_buf.reg
    );
}

void dbg_print_clk()
{
    btn_printf(btn_return_clk());
}
