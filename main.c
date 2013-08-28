#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "button.h"
#include "gpio.h"
#include "lcd.h"
#include "lcd_cgram.h"

static void set_colour(const char* s);
static void set_cursor(const char* s);
static void help();
static void lcd_off();

int verbose_flag = 0;
int lcd_off_flag = 0;
uint8_t lcd_opts = 0;

int main(int argc, char* const* argv)
{
    int s;
    int opts_index = 0;

    char* short_opts = "hc:u:";
    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"led", required_argument, NULL, 'l'},
        {"cursor", required_argument, NULL, 'u'},
        {"off", no_argument, &lcd_off_flag, 1},
        {"verbose", no_argument, &verbose_flag, 1},
        {0, 0, 0, 0}
    };

    while( (s = getopt_long(
        argc, argv, short_opts, long_opts, &opts_index)) > 0 ) {
        switch(s){
            case 'h':
                printf("help\n");
                break;
            case 'c':
                printf("colour: %s\n", optarg);
                break;
            case 'u':
                printf("cursor: %s\n", optarg);
                break;
            case '?':
                break;
        }
    };

    GPIO_open();
    LCD_init(lcd_opts);

    if(lcd_off_flag){
        if(verbose_flag){
            printf("Turning off LCD\n");
        }
        LCD_colour(Black);
    }

    /* printing off the remaining arguments */
    while(optind < argc){
        LCD_wrap_printf("%s ", argv[optind++]);
    }
    return 0;
}