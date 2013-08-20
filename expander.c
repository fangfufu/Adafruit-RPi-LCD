#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

//#include "lcd.h"

#include "expander.h"

/* Are you using Raspberry Pi Rev 1? */
// #define RPI_REV1
#ifndef RPI_REV1
#define I2C_BUS "/dev/i2c-1"
#else
#define I2C_BUS "/dev/i2c-0"
#endif

/* The address for the expansion board */
#define I2C_ADDR 0x20

/* Initial address of the IOCON register, (pg.5 of the data sheet) */
#define IOCON_BANK_0    0x0A

/* Set Bank = 1, so registers for ports are grouped together
 * Set SEQOP = 1 to disable sequential operation.
 * (pg 18 of the data sheet) */
#define IOCON_CONFIG    ( ( 1 << 5 ) | ( 1 <<7 ) )

/* IO direction, 0 = output, 1 = input (pg. 12 of the data sheet) */
#define PORTA_IODIR         0x1F
#define PORTB_IODIR         0x00

/* Input polarity, 1 = inverse, 0 = normal (pg 13 of the data sheet) */
#define PORTA_IOPOL             0x1F
#define PORTB_IOPOL             0x00

/* GPIO pull-up, 1 = enabled, 0 = disabled (pg 19 of the data sheet) */
#define PORTA_GPPU              0x1F
#define PORTB_GPPU              0x00

/* File descriptor for the I2C bus */
static int fd;
/* Initialisation status */
static int status = 0;

int exp_init()
{
    if (status == 1) {
        printf("I/O expander already initialised.\n");
        return 0;
    }
    status = 1;
    fd = open(I2C_BUS, O_RDWR);
    if (fd == -1) {
        printf("open error: %s\n", strerror(errno));
        return -1;
    }
    if (ioctl(fd, I2C_SLAVE, I2C_ADDR) == -1) {
        printf("ioctl error: %s\n", strerror(errno));
        return -1;
    }
    /* Change the addressing mode of the IO expander */
    int r = exp_write(PortA, IOCON_BANK_0, IOCON_CONFIG);
    r += exp_write(PortA, IODIR, PORTA_IODIR);
    r += exp_write(PortB, IODIR, PORTB_IODIR);
    r += exp_write(PortA, IOPOL, PORTA_IOPOL);
    r += exp_write(PortB, IOPOL, PORTB_IOPOL);
    r += exp_write(PortA, GPPU, PORTA_GPPU);
    r += exp_write(PortB, GPPU, PORTB_GPPU);
    /* Turn off all LEDs on the LCD */
    r += exp_write(PortA, GPIO, (1 << 7) | (1 << 6));
    r += exp_write(PortB, GPIO, 1);
    if (r == 0) {
        return r;
    }
    status = 0;
    printf("I/O expander initialisation Error: %d\n", r);
    return r;
}

int exp_close()
{
    int t = close(fd);
    if (t == 0) {
        fd = status = 0;
    } else {
        printf("I/O expander close down error: %s\n",strerror(errno));
    }
    return t;
}

int exp_write(Port port, Reg reg, uint8_t val)
{
    if (!status) {
        printf("exp_write(): error: not initialised.\n");
        return -1;
    }
    uint8_t buf[2];
    buf[0] = port + reg;
    buf[1] = val;
    int r = write(fd, buf, sizeof(buf));
    if ( r == 2 ) {
        return 0;
    } else if( r == -1 ) {
        printf("exp_write(): write() error: %s\n", strerror(errno));
    } else {
        printf("exp_write(): write() error: %d\n", r);
    }
    return r;
}

uint8_t exp_read(Port port, Reg reg)
{
    if (!status) {
        printf("exp_read(): error: not initialised.\n");
        return -1;
    }
    uint8_t buf;
    int addr = port + reg;
    int r = write(fd, &addr, 1);
    if(r == -1) {
        printf("exp_read(): write() error: %s\n", strerror(errno));
        return r;
    } else if (r != 1) {
        printf("exp_read(): write() error: %d\n", r);
    }
    r = read(fd, &buf, 1);
    if (r == -1) {
        printf("exp_read(): read() error: %s\n", strerror(errno));
    } else if (r != 1){
        printf("exp_read(): read() error: %d\n", r);
    }
    return buf;
}