#
# Makefile
#

CIRCLEHOME = ../..

OBJS	= main.o kernel.o

LIBS	= $(CIRCLEHOME)/app/lib/template/libtempl.a \
	  $(CIRCLEHOME)/lib/libcircle.a

CFLAGS = -O0

include ../Rules.mk

-include $(DEPS)

flash:
	flashy /dev/tty.usbserial-FTC7WYB3 flash kernel7l.img
.PHONY: flash

# ctrl + K > a > y to exit
serial:
	screen /dev/tty.usbserial-FTC7WYB3 9600
.PHONY: serial

