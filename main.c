#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "button.h"
#include "gpio.h"
#include "lcd.h"
#include "lcd_cgram.h"



int main(int argc, char** argv)
{
    uint8_t lcd_opts = 0;
    char* shortopts = "buhoc:"
    struct option longopts[] = {
        {"blink", no_argument, NULL, 'b'},
        {"cursor", no_argument, NULL, 'u'},
        {"help", no_argument, NULL, 'h'},
        {"off", no_argument, NULL, 'o'},
        {"colour", required_argument, NULL, 'c'},
        {NULL, NULL, NULL, NULL}
    }
    GPIO_open();

    LCD_init(0);
    LCD_printf("%s", argv[1]);
}