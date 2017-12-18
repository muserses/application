#
# Makefile
# muserses, 2017-01-14 18:01
#

MK := chip.mk
MKPATH := ./mkfile


ifndef HC_MACH
ifneq ("$(wildcard $(MK))", "")
HC_MACH := $(shell cat $(MK))
endif
else
#$(shell rm -f $(MK))
$(shell echo $(HC_MACH) > $(MK))
endif

ifneq ($(MAKECMDGOALS), clean)
ifeq ($(HC_MACH), )
$(info Please type full command for the first build.)
$(info For example, make HC_MACH=mt6256 all)
endif
include $(MKPATH)/$(HC_MACH).mk
endif


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

DEFS=-DCHIPID=$(CHIP)


all: $(BIN)


$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(DEFS) -c $< -o $@

clean:
	rm -f $(MK)
	rm -f *.o $(BIN)

# vim:ft=make
#
