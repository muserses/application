#
# Makefile
# muserses, 2017-01-14 18:01
#

#HC_MACH ?=
MK := ./chip.mk
MKPATH := ./mkfile


ifneq ($(MAKECMDGOALS), clean)
-include $(MK)
endif


.PHONY: all prepare clean

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


all: prepare $(BIN)

prepare:
	@if [ ! -f $(MK) ]; then \
		echo "Make parameter file does not exist!" && exit 1; \
	fi

%.mk:
	@if [ ! -f $(MKPATH)/$@ ]; then \
		echo "Make parameter file $@ does not exist!" && exit 1; \
	else \
		echo "Create mk"; \
		cp $(MKPATH)/$@ $(MK); \
	fi

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(DEFS) -c $< -o $@

clean:
	rm -f *.o $(BIN)


# vim:ft=make
#
