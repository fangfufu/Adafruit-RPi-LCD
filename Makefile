ifeq ($(shell uname -m),armv6l)
  CC = cc
else
  CC = arm-linux-gnueabi-gcc-4.7
endif

CC=cc

CFLAGS = -std=gnu99 -Wall -Wextra -O2 -pedantic

ifneq ($(RPI_REV1),)
CFLAGS += -DRPI_REV1
endif

LDFLAGS =
SOURCES = main.c gpio.c lcd.c button.c lcd_lowlevel.c lcd_cgram.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = adafruit-rpi-lcd
INSTALL_PATH = /usr/local/bin

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

depend: .depend
.depend: $(SOURCES)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ -MF  ./.depend
include .depend

.PHONY: clean
clean:
	rm *.o $(EXECUTABLE) -rf

.PHONY: install
install:
	install -m 0755 $(EXECUTABLE) /usr/local/bin

.PHONY: uninstall
uninstall:
	rm -rf /usr/local/bin/$(EXECUTABLE)
