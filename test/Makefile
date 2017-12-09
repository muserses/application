#
# Makefile
# muserses, 2017-01-14 18:01
#

MK := chip.mk
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

DEFS=-DCHIPID=$(CHIP)


all: prepare $(BIN)

prepare:
	@if [ ! -f $(MK) ]; then \
		echo "chip.mk file does not exist!" && exit 1; \
	fi

%.mk:
	@echo "Make file = $@"
	@if [ $@ != $(MK) ]; then \
		if [ ! -f $(MKPATH)/$@ ]; then \
			echo "Make parameter file $@ does not exist!" && exit 1; \
		else \
			echo "Create make parameter file!"; \
			cp $(MKPATH)/$@ $(MK); \
		fi \
	fi

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(DEFS) -c $< -o $@

clean:
	rm -f $(MK)
	rm -f *.o $(BIN)

# vim:ft=make
#
