#
# Makefile
# muserses, 2017-01-14 18:01
#

.PHONY: all clean

#ARCH = arm
#CROSS_COMPILE = 
CC=arm-linux-gnueabihf-gcc-4.8.3
#CC=arm-none-eabi-gcc-4.8.3
CFLAGS=-Wall -fopenmp -g 

BIN=test
OBJS=test.o
INC=
LIBS=

DEFS=-DCHIPID=MT6258


all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(DEFS) -c $< -o $@

clean:
	rm -f *.o $(BIN)


# vim:ft=make
#
