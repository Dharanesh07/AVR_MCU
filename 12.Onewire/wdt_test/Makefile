CPU=atmega328p
CLOCK=2000000UL

CPP=avr-gcc
OBJCOPY=avr-objcopy
STRIP=avr-strip
SIZE=avr-size
AVRDUDE=avrdude
AVRDUDE_OPTIONS=-B5 -c avrispmkII -p ${CPU}
PORT=/dev/ttyUSB0

NAME=harvester

EMPTY=

SOURCES= main.c \
		usart.c \
		ds1820.c \
		adc.c \
		timer.c \
        ${EMPTY}
              
HEADERS= usart.h \
		ds1820.h \
		adc.h \
		timer.h   
        ${EMPTY}

OBJECTS=$(shell echo "${SOURCES}" | sed -e 's/\.c/\.o/g')

CCFLAGS= -mmcu=${CPU} -O1 -std=c99 -DF_CPU=${CLOCK} -Wall
LIBS=-Wl,-u,vfprintf -lprintf_flt -lm

.PHONY: all

all: ${NAME}
	${SIZE} ${NAME}


${NAME}:${OBJECTS} 
	${CPP} ${CCFLAGS} ${LIBS} ${OBJECTS} -o $@
	${OBJCOPY} -O ihex $@ $@.hex
	${OBJCOPY} -O elf32-avr $@ $@.elf


clean:
	rm -f ${OBJECTS}
	rm -f ${NAME_TINY}.hex ${NAME}.hex
	rm -f ${NAME_TINY}.elf ${NAME}.elf
	rm -f ${NAME_TINY} ${NAME}

upload: ${NAME}
	avrdude -v -patmega328p -carduino -P ${PORT} -b19200 -D -Uflash:w:${NAME}.elf
	
${OBJECTS}:%.o:%.c ${HEADERS}
	${CPP} ${CCFLAGS} $< -c -o $@
