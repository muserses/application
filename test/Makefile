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
$(shell rm -f $(MK))
endif

ifneq ($(HC_MACH), )
ifneq ($(MAKECMDGOALS), clean)
include $(MKPATH)/$(HC_MACH).mk
endif
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
	@if [ "$(HC_MACH)" = "" ]; then \
		echo "HC_MACH has not been specified ever!"; \
		echo "Please type full command for the first build, for example:"; \
		echo "make HC_MACH=mt6256 all"; \
		echo ""; \
		exit 1; \
	else \
		echo $(HC_MACH) > $(MK); \
	fi

#	@if [ ! -f $(MK) ]; then \
#		echo "chip.mk file does not exist!" && exit 1; \
#	fi

#%.mk:
#	@echo "Make file = $@"
#	@if [ $@ != $(MK) ]; then \
#		if [ ! -f $(MKPATH)/$@ ]; then \
#			echo "Make parameter file $@ does not exist!" && exit 1; \
#		else \
#			echo "Create make parameter file!"; \
#			cp $(MKPATH)/$@ $(MK); \
#		fi \
#	fi

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(DEFS) -c $< -o $@

clean:
	rm -f $(MK)
	rm -f *.o $(BIN)

# vim:ft=make
#
