/**
 * @file lcd.c
 * @details
 * - The data pins for the LCD are connected in the reverse order on the
 * I/O expander. Hence we need to manually map pins.
 * - the send/receive nibble commands might cause misalignment in command
 * sequences. Please use them with caution.
 */
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#include "gpio.h"

#include "lcd.h"

/* Opcode definitions - pg.24 of the datasheet          */

/** Special initialisation code for 4-bit mode          */
#define FOUR_BIT_MODE           0x02

/** Clear display                                       */
#define CLEAR                   0x01

/** Return home                                         */
#define HOME                    0x02

/**
 * @brief Function set
 * @details
 * - Interface length = 4 bits,
 * - Display lines = 2,
 * - Character font = 5 x 8 dots (seems to make no difference)
 */
#define FUNCTION_SET            0x28

/** Set Character Generation RAM (CGRAM) address
 */
#define CGRAM                   0x40

/** Set Display Data RAM (DDRAM) address                */
#define DDRAM                   0x80

/* Other bits and pieces                                */

/** The number of characters on each line of the LCD    */
#define LCD_LENGTH         16

/** The number of characters DDRAM can store            */
#define DDRAM_LENGTH        80

/** GPIO Port B polarity for read                       */
#define LCD_READ                0x1E
/** GPIO Port B polarity for write                      */
#define LCD_WRITE               0x00

/** Busy Flag -- DB7 pin                                */
#define BUSY_FLAG               0x80

typedef union {
    uint8_t byte;
    struct {
        unsigned int _0  : 1;
        unsigned int _1  : 1;
        unsigned int _2  : 1;
        unsigned int _3  : 1;
        unsigned int _4  : 1;
        unsigned int _5  : 1;
        unsigned int _6  : 1;
        unsigned int _7  : 1;
    } bit;
} Byte;

int LCD_DISPLAY_SHIFT = 0;

/** LCD initialisation status flag */
static int g_init = 0;

/** Flag for indicating that the address register is pointed to CGRAM */
static int g_toCGRAM = 0;

/** DDRAM address before switching to CGRAM */
static uint8_t g_DDRAM_addr = 0;

/**
 * @brief Send a nibble to one of the LCD registers
 * @param[in] data please put the data in the lower nibble.
 * @param[in] RS
 * - 0: Select instruction register
 * - 1: Select data register
 * @return 0, on success
 * @warning: This function DOES NOT change the GPIO direction, so it should
 * not be called on its own.
 */
static int write_nibble(uint8_t data, int RS)
{

    int r;
    Byte b;

    b.byte = data;
    /* Set control pins */
    GPIOB_buf.pin.RS = RS;
    GPIOB_buf.pin.R = 0;

    /* Set data pins */
    GPIOB_buf.pin.DB7 = b.bit._3;
    GPIOB_buf.pin.DB6 = b.bit._2;
    GPIOB_buf.pin.DB5 = b.bit._1;
    GPIOB_buf.pin.DB4 = b.bit._0;

    /* Flip the E-pin.                                          */
    /* Note that data is clocked in at the lowering edge.       */
    GPIOB_buf.pin.E = 1;
    r = GPIO_write(PortB);
    GPIOB_buf.pin.E = 0;
    r += GPIO_write(PortB);
    return r;
}

/**
 * @brief read a nibble from one of the LCD registers
 * @param[in] RS
 * - 0: Select instruction register
 * - 1: Select data register
 * @return the data will be in the lower nibble.
 * @note please refer to the timing diagram at pg. 22
 */
static uint8_t read_nibble(int RS)
{
    Byte b;
    b.byte = 0;

    /* Set control pins */
    GPIOB_buf.pin.RS = RS;
    GPIOB_buf.pin.R = 1;

    /* Flip the E-pin.                                          */
    /* Note that data is clocked in at the rising edge.         */
    GPIOB_buf.pin.E = 0;
    GPIO_write(PortB);
    GPIOB_buf.pin.E = 1;
    GPIO_write(PortB);

    /* The actual read operation */
    GPIO_read(PortB);

    /* Copy back the data */
    b.bit._3 = GPIOB_buf.pin.DB7;
    b.bit._2 = GPIOB_buf.pin.DB6;
    b.bit._1 = GPIOB_buf.pin.DB5;
    b.bit._0 = GPIOB_buf.pin.DB4;
    return b.byte;
}

/**
 * @brief Send a byte to the LCD screen
 * @details This function calls write_nibble twice.
 * @return
 * - on success: 0
 * - on error: accumulated error from the write_nibble call.
 */
int write_byte(uint8_t data, int RS)
{
    if (!g_init) {
        printf("write_byte: error: LCD is not initialised!\n");
        return -1;
    }
    int r;
    r =  GPIO_direction(PortB, LCD_WRITE);
    r += write_nibble (data >> 4, RS);
    r += write_nibble(data, RS);
    return r;
}

/**
 * @brief read a byte from one of the LCD registers
 * @param[in] RS
 * - 0: Select instruction register
 * - 1: Select data register
 */
static uint8_t read_byte(int RS)
{
    if (!g_init) {
        printf("read_byte: error: LCD is not initialised!\n");
        return -1;
    }
    uint8_t r;
    GPIO_direction(PortB, LCD_READ);
    r  = read_nibble(RS) << 4;
    r |= read_nibble(RS);
    return r;
}

/**
 * @brief Poll LCD until it is no longer busy
 */
static void busy_wait()
{
    while (read_byte(0) & BUSY_FLAG)
        ;
}

/**
 * @brief restore DDRAM address
 * @return 0, if the address counter is now point to DDRAM.
 */
static int DDRAM_addr_restore()
{
    if (g_toCGRAM != 0) {
        g_toCGRAM = 0;
        LCD_cmd(DDRAM|g_DDRAM_addr);
    }
    return g_toCGRAM;
}

/**
 * @brief LCD DDRAM self test
 * @details Print DDRAM_LENGTH characters to DDRAM, and read them back. If they
 * are the same, then the test is successful.
 */
static int self_test()
{
    int i;
    int s = 0;
    char c, d;
    LCD_clear();
    for (i = 0; i < DDRAM_LENGTH + 1 ; i++ ){
        c = LCD_putchar('0' + i);
        LCD_cursor_move(-1);
        d = LCD_getchar();
        if (c == d) {
            s++;
        }
    }
    if (s == DDRAM_LENGTH) {
        LCD_clear();
        return 0;
    }
    return -1;
}

int LCD_init()
{
    if (g_init == 1) {
        printf("LCD_init: LCD is already initialised.\n");
        return 0;
    }
    g_init = 1;
    LCD_cmd(FOUR_BIT_MODE);
    LCD_cmd(FUNCTION_SET);
    LCD_cmd(DISPLAY_SET | DISPLAY_ON | CURSOR_ON | CURSOR_BLINK_ON);
    LCD_cmd(ENTRY_MODE_SET|INCREMENT);
    int r = self_test();
    if (r == 0){
        return r;
    }
    g_init = 0;
    printf("LCD_init: LCD initialisation failed\n");
    return r;
}

int LCD_cmd(uint8_t cmd)
{
    return write_byte(cmd, 0);
}

int LCD_putchar (char c)
{
    switch(c) {
        case '\r':
            return LCD_cmd(DDRAM | ((LCD_cursor_addr() < 0x27) ? 0x00 : 0x40));
            break;
        case '\n':
            return LCD_cmd(DDRAM | ((LCD_cursor_addr() > 0x27) ? 0x00 : 0x40));
            break;
    }
    if (write_byte(c, 1) != 0) {
        return EOF;
    }
    return (unsigned char) c;
}

/**
 * @brief read the character in DDRAM
 */
char LCD_getchar()
{
    DDRAM_addr_restore();
    char c = read_byte(1);
    busy_wait();
    return c;
}

uint8_t LCD_cursor_addr()
{
    DDRAM_addr_restore();
    g_DDRAM_addr = read_byte(0);
    return g_DDRAM_addr;
}

int LCD_clear()
{
    int r = LCD_cmd(CLEAR);
    busy_wait();
    LCD_DISPLAY_SHIFT = 0;
    return r;
}

int LCD_home()
{
    int r = LCD_cmd(HOME);
    LCD_DISPLAY_SHIFT = 0;
    return r;
}

int LCD_line_clear()
{
    int r = LCD_putchar('\r');
    for (int i =0; i < LCD_LENGTH; i++){
        r += LCD_putchar(' ');
    }
    r += LCD_putchar('\r');
    return r;
}

int LCD_printf(const char *format, ...)
{
    va_list arg;
    char s[DDRAM_LENGTH + 1];

    va_start(arg, format);
    int n = vsnprintf(s, DDRAM_LENGTH + 1, format, arg);
    va_end(arg);

    for (int i = 0; s[i] != '\0'; i++){
        LCD_putchar(s[i]);
    }

    return n;
}

int LCD_wrap_printf(const char *format, ...)
{
    LCD_clear();

    va_list arg;
    char s[2 * LCD_LENGTH + 1];

    va_start(arg, format);
    int n = vsnprintf(s, 2 * LCD_LENGTH + 1, format, arg);
    va_end(arg);

    for (int i = 0; s[i] != '\0'; i++){
        if (i == LCD_LENGTH) {
            LCD_putchar('\n');
        }
        LCD_putchar(s[i]);
    }

    return n;
}

int LCD_cursor_move(int n)
{
    int i;
    if (n > 0){
        for (i = 0; i < n; i++) {
            LCD_cmd(SHIFT|CURSOR|RIGHT);
        }
    } else {
        for (i = 0; i > n; i--) {
            LCD_cmd(SHIFT|CURSOR|LEFT);
        }
    }
    return LCD_cursor_addr();
}

int LCD_display_shift(int n)
{
    int i = 0;
    if (n > 0){
        for (i = 0; i < n; i++) {
            LCD_cmd(SHIFT|DISPLAY|RIGHT);
            LCD_DISPLAY_SHIFT++;
        }
    } else {
        for (i = 0; i > n; i--) {
            LCD_cmd(SHIFT|DISPLAY|LEFT);
            LCD_DISPLAY_SHIFT--;
        }
    }
    LCD_DISPLAY_SHIFT %= 40;
    return LCD_DISPLAY_SHIFT;
}

/**
 * @note For the LEDs on the LCD display, logic value '1' actually turns them
 * off, whereas logic value '0' turns them on.
 */
int LCD_colour(Colour colour)
{
    switch(colour) {
        case Black:
            GPIOA_buf.pin.RED = 1;
            GPIOA_buf.pin.GREEN = 1;
            GPIOB_buf.pin.BLUE = 1;
            break;
        case Red:
            GPIOA_buf.pin.RED = 0;
            GPIOA_buf.pin.GREEN = 1;
            GPIOB_buf.pin.BLUE = 1;
            break;
        case Yellow:
            GPIOA_buf.pin.RED = 0;
            GPIOA_buf.pin.GREEN = 0;
            GPIOB_buf.pin.BLUE = 1;
            break;
        case Green:
            GPIOA_buf.pin.RED = 1;
            GPIOA_buf.pin.GREEN = 0;
            GPIOB_buf.pin.BLUE = 1;
            break;
        case Cyan:
            GPIOA_buf.pin.RED = 1;
            GPIOA_buf.pin.GREEN = 0;
            GPIOB_buf.pin.BLUE = 0;
            break;
        case Blue:
            GPIOA_buf.pin.RED = 1;
            GPIOA_buf.pin.GREEN = 1;
            GPIOB_buf.pin.BLUE = 0;
            break;
        case Magenta:
            GPIOA_buf.pin.RED = 0;
            GPIOA_buf.pin.GREEN = 1;
            GPIOB_buf.pin.BLUE = 0;
            break;
        case White:
            GPIOA_buf.pin.RED = 0;
            GPIOA_buf.pin.GREEN = 0;
            GPIOB_buf.pin.BLUE = 0;
            break;
    }
    int r;
    r = GPIO_write(PortA);
    r += GPIO_write(PortB);

    if (r == 0) {
        return r;
    }
    printf("LCD_colour: Colour change error: %d\n", r);
    return r;
}