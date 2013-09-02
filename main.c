#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "button.h"
#include "gpio.h"
#include "lcd.h"
#include "lcd_cgram.h"

static int set_colour(const char* s);
static int set_cursor(const char* s);
static int help();

static int lcd_off = 0;
static uint8_t lcd_opts = 0;
static int verbose = 0;

int main(int argc, char* const* argv)
{
    int s;
    int opts_index = 0;

    char* short_opts = "hc:u:v";
    struct option long_opts[] = {
        {"off", no_argument, &lcd_off, 1},
        {"verbose", no_argument, &verbose, 1},
        {"help", no_argument, NULL, 'h'},
        {"colour", required_argument, NULL, 'c'},
        {"cursor", required_argument, NULL, 'u'},
        {0, 0, 0, 0}
    };

    GPIO_open();
    LCD_init(0);

    int option_index;
    
    while( (s = getopt_long(
        argc, argv, short_opts, long_opts, &opts_index)) > 0 ) {
        switch(s){
            case 'h':
                printf("help\n");
                break;
            case 'c':
                set_colour(optarg);
                break;
            case 'u':
                printf("cursor: %s\n", optarg);
                break;
            case '?':
                break;
        }
    };

    printf("main verbose: %d\n", verbose);

    if(lcd_off){
        if(verbose){
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

int set_colour(const char* carg)
{
    printf("set_colour verbose: %d\n", verbose);

    Colour c = Black;

    if(!strncasecmp(carg, "White", 5)) {
        c = Black;
    } else if (!strncasecmp(carg, "Red", 3)) {
        c =  Red;
    } else if (!strncasecmp(carg, "Yellow", 6)) {
        c =  Yellow;
    } else if (!strncasecmp(carg, "Green", 5)) {
        c =  Green;
    } else if (!strncasecmp(carg, "Cyan", 4)) {
        c =  Cyan;
    } else if (!strncasecmp(carg, "Blue", 4)) {
        c =  Blue;
    } else if (!strncasecmp(carg, "Magenta", 7)) {
        c =  Magenta;
    } else if (!strncasecmp(carg, "White", 5)) {
        c =  White;
    } else {
        fprintf(stderr, "string_to_colour error: Invalid colour");
    }

    if(verbose) {
        printf("Setting LCD colour to: %s\n", carg);
    }
    return LCD_colour(c);
}