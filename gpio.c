#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "gpio.h"

/** Set Raspberry Pi Board version */
// #define RPI_REV1
#ifndef RPI_REV1
#define I2C_BUS "/dev/i2c-1"
#else
#define I2C_BUS "/dev/i2c-0"
#endif

/** The address for the expansion board */
#define I2C_ADDR                0x20

/** Initial address of the IOCON register, (pg.5 of the datasheet) */
#define IOCON_BANK_0            0x0A

/**      Address for IO Direction Register            */
#define IODIR                   0x00
/**      Address for Input Polarity Register          */
#define IOPOL                   0x01
/**      Address for General Configuration Register   */
#define IOCON                   0x05
/**      Address for Pull-up  Register                */
#define GPPU                    0x06
/**      Address for GPIO Register                    */
#define GPIO                    0x09

/**
 * @brief IOCON configuration
 * @details pg 18 of the data sheet
 * - Set Bank = 1 to registers for ports are grouped together,
 * - Set SEQOP = 1 to disable sequential operation.
 */
#define IOCON_CONFIG            (1 << 5) | (1 <<7)

/**
 * @brief IO direction,
 * @details 0 = output, 1 = input, (pg.12 of the datasheet)
 */
#define PORTA_IODIR             0x1F
#define PORTB_IODIR             0x00

/**
 * @brief Input polarity
 * @details 1 = inverse, 0 = normal (pg.13 of the datasheet)
 */
#define PORTA_IOPOL             0x1F
#define PORTB_IOPOL             0x00

/**
 * @brief GPIO pull-up
 * @details 1 = enabled, 0 = disabled (pg.19 of the datasheet)
 */
#define PORTA_GPPU              0x1F
#define PORTB_GPPU              0x00

/** Initial values for the GPIO ports */
#define GPIOA_INIT              0
#define GPIOB_INIT              0

/** File descriptor for the I2C bus */
static int fd;

/** Initialisation status */
static int g_init = 0;

/** Buffers for GPIO ports */
GPIOA_BUF_t GPIOA_buf;
GPIOB_BUF_t GPIOB_buf;

/**
 * @brief Write a byte to a register in the expander
 * @return 0, on success
 */
static int exp_write(Port port, uint8_t reg, uint8_t val)
{
    if (!g_init) {
        printf("exp_write: error: is not initialised.\n");
        return -1;
    }
    uint8_t buf[2];
    buf[0] = port + reg;
    buf[1] = val;
    int r;
    r = write(fd, buf, sizeof(buf));
    if ( r == 2 ) {
        return 0;
    } else if( r == -1 ) {
        printf("exp_write: write() error: %s\n", strerror(errno));
    } else {
        printf("exp_write: write() error: %d\n", r);
    }
    return r;
}

/**
 * @brief Read a single byte from a register
 */
static uint8_t exp_read(Port port, uint8_t reg)
{
    if (!g_init) {
        printf("exp_read: error: is not initialised.\n");
        return -1;
    }
    uint8_t buf;
    int addr = port + reg;
    int r = write(fd, &addr, 1);

    if(r == -1) {
        printf("exp_read: write error: %s\n", strerror(errno));
        return r;
    } else if (r != 1) {
        printf("exp_read: write error: %d\n", r);
    }
    r = read(fd, &buf, 1);
    if (r == -1) {
        printf("exp_read: read error: %s\n", strerror(errno));
    } else if (r != 1){
        printf("exp_read: read error: %d\n", r);
    }
    return buf;
}

int GPIO_open()
{
    if (g_init == 1) {
        printf("GPIO_open: I/O expander is already initialised.\n");
        return 0;
    }
    g_init = 1;
    fd = open(I2C_BUS, O_RDWR);
    if (fd == -1) {
        printf("GPIO_open: open error: %s\n", strerror(errno));
        return -1;
    }
    if (ioctl(fd, I2C_SLAVE, I2C_ADDR) == -1) {
        printf("GPIO_open: ioctl error: %s\n", strerror(errno));
        return -1;
    }
    /* Configure IO expander */
    int r;
    r = exp_write(PortA, IOCON_BANK_0, IOCON_CONFIG);
    r += GPIO_direction(PortA, PORTA_IODIR);
    r += GPIO_direction(PortB, PORTB_IODIR);
    r += exp_write(PortA, IOPOL, PORTA_IOPOL);
    r += exp_write(PortB, IOPOL, PORTB_IOPOL);
    r += exp_write(PortA, GPPU, PORTA_GPPU);
    r += exp_write(PortB, GPPU, PORTB_GPPU);

    /* Configure the GPIO pins */
    GPIOA_buf.reg = GPIOA_INIT;
    GPIOB_buf.reg = GPIOB_INIT;
    r += GPIO_write(PortA);
    r += GPIO_write(PortB);
    if (r == 0) {
        return r;
    }
    g_init = 0;
    printf("GPIO_open: I/O expander initialisation Error: %d\n", r);
    return r;
}

int GPIO_close()
{
    int t = close(fd);
    if (t == 0) {
        fd = g_init = 0;
    } else {
        printf("GPIO_close: I/O expander close down error: %s\n",strerror(errno));
    }
    return t;
}

int GPIO_direction(Port port, uint8_t polarity)
{
    if (port == PortA) {
        return exp_write(PortA, IODIR, polarity);
    } else {
        return exp_write(PortB, IODIR, polarity);
    }
}

int GPIO_write(Port port)
{
    if (port == PortA) {
        return exp_write(PortA, GPIO, GPIOA_buf.reg);
    } else {
        return exp_write(PortB, GPIO, GPIOB_buf.reg);
    }
}

uint8_t GPIO_read(Port port)
{
    if (port == PortA) {
        return GPIOA_buf.reg = exp_read(PortA, GPIO);
    } else {
        return GPIOB_buf.reg = exp_read(PortB, GPIO);
    }
}
