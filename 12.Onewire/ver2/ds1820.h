#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#define ONEWIRE_PIN  PB2    
#define ONEWIRE_DDR  DDRB   
#define ONEWIRE_PORT PORTB  
#define ONEWIRE_PINR PINB   

#define START_CONV       0x44
#define READ_SCRATCH_PAD 0xBE
#define SKIP_ROM         0xCC

typedef struct {
    uint8_t lsb;
    uint8_t msb;
} ds1820_result;

uint8_t onewire_rst();

void  onewire_writebit(uint8_t bit);

uint8_t onewire_readbit();

void onewire_writebyte(uint8_t byte);

uint8_t onewire_readbyte();

void ds1820_pwron();

void ds1820_pwroff();

ds1820_result ds1820_readtemp();

float convert_temp(uint8_t msb, uint8_t lsb);
