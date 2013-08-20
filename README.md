Adafruit-Raspberry-Pi-RGB-LCD-Plate-C-Library
=============================================
/**
@mainpage
\section intro Introduction
This is a C library for driving the Adafruit Raspberry Pi RGB LCD Plate.
(link: http://www.adafruit.com/products/1110) It should work for the monochrome
version with minor modification.

The MCP23017 I2C I/O expander has two 8-bit GPIO ports. I have included the
pin connection table below for reference.

\section table MCP23017 Pin table:

Pin No. |7       |6       |5       |4       |3       |2       |1       |0
--------|--------|--------|--------|--------|--------|--------|--------|-------
 GPIO A:|GREEN   |RED     |        |LEFT    |UP      |DOWN    |RIGHT   |SELECT
 GPIO B:|RS      |RW      |E       |DB4     |DB5     |DB6     |DB7     |BLUE
*/
