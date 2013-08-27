Adafruit-Raspberry-Pi-RGB-LCD-Plate-C-Library
=============================================
@mainpage
@section intro Introduction
This is a C library for driving the Adafruit Raspberry Pi RGB LCD Plate.
It should work for the monochrome version with minor modification.

The idea is that you include the header file in your project, so you can
drive the LCD very easily. You can indeed do a "printf" to the LCD. I don't
think many libraries have such nice feature. This library also perform
proper error checking

You can generate a nice set of html documentation by issuing the following
command:
@code
doxygen Doxyfile
@endcode

If you don't have Doxygen, and you are on Debian-based system, you can install
Doxygen by issuing:
@code
sudo apt-get install doxygen
@endcode

@section header Header files
Here is a brief description of what each header file do:
- gpio.h: functions related to the I2C GPIO expander
- button.h: functions related to GPIO buttons
- lcd.h: functions related to the normal operation of the LCD.
- lcd_cgram.h: functions for creating custom characters on the LCD.
- lcd_lowlevel.h: low level LCD functions, you should never need to included
this header.

@section Initialisation
To initialise the LCD plate, you need to include gpio.h and lcd.h. You can
initialise the system by calling the following functions:
@code
GPIO_open() // Open the GPIO ports for read/write operations.
LCD_init() // Initialise the LCD and perform self-check.
@endcode

@section extra Additional Information

@subsection Vendors
You can buy the LCD plate from the following places:
- Adafruit:
http://www.adafruit.com/products/1110
- UK Distributor (I bought mine there):
https://www.modmypi.com/raspberry-pi-RGB-negative-16x2-LCD-kit

@subsection datasheet Datasheets
Hopefully my code is clear enough so you don't need datasheets. But if you care
about how it all works, here is the additional information.

- MCP23017 I2C I/O expander:
http://ww1.microchip.com/downloads/en/devicedoc/21952b.pdf
- HD44780 LCD Controller (ROM Code A00):
https://www.adafruit.com/datasheets/HD44780.pdf

@subsection table Pin Table
The MCP23017 I2C I/O expander has two 8-bit GPIO ports. I have included the
pin connection table below for reference.

Pin No. |7       |6       |5       |4       |3       |2       |1       |0
--------|--------|--------|--------|--------|--------|--------|--------|-------
 GPIO A:|GREEN   |RED     |        |LEFT    |UP      |DOWN    |RIGHT   |SELECT
 GPIO B:|RS      |R/!W    |E       |DB4     |DB5     |DB6     |DB7     |BLUE
