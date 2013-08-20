CC = arm-linux-gnueabi-gcc-4.7
CFLAGS = -std=gnu99 -g -Wall -Wextra -O2 -pedantic
LDFLAGS =
SOURCES = main.c debug.c expander.c lcd.c button.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = LCD

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
	rm *.o $(EXECUTABLE) html -rf

.PHONY: html
html:
	doxygen Doxyfile