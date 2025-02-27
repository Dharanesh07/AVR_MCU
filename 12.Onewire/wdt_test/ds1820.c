#include "ds1820.h"



// Reset the OneWire bus and check for sensor
uint8_t onewire_rst() {
   
    // Set as output
    ONEWIRE_DDR |= (1 << ONEWIRE_PIN);   
    // Pull low
    ONEWIRE_PORT &= ~(1 << ONEWIRE_PIN); 
    _delay_us(480);                      

    // Release (set as input)
    ONEWIRE_DDR &= ~(1 << ONEWIRE_PIN);  
    _delay_us(70);

    // Check if sensor pulls line low
    uint8_t presence = (ONEWIRE_PINR & (1 << ONEWIRE_PIN)) == 0;  
    _delay_us(410);                      

    return presence; 
}

//Writes single bit to the OneWire bus
void onewire_writebit(uint8_t bit) {
    // Set as output
    ONEWIRE_DDR |= (1 << ONEWIRE_PIN);   
    // Pull low
    ONEWIRE_PORT &= ~(1 << ONEWIRE_PIN); 
    _delay_us(2);

    if (bit) {
        // Release line (pull-up)
        ONEWIRE_DDR &= ~(1 << ONEWIRE_PIN); 
    }

    _delay_us(60);
    // Ensure line is released
    ONEWIRE_DDR &= ~(1 << ONEWIRE_PIN); 
}

//Reads single bit from the OneWire bus
uint8_t onewire_readbit() {
    // Set as output
    ONEWIRE_DDR |= (1 << ONEWIRE_PIN);   
    // Pull low
    ONEWIRE_PORT &= ~(1 << ONEWIRE_PIN); 
    _delay_us(2);
    
    // Release line
    ONEWIRE_DDR &= ~(1 << ONEWIRE_PIN);  
    _delay_us(10); 
    
    //check whether the bit is high or low
    uint8_t bit = (ONEWIRE_PINR & (1 << ONEWIRE_PIN)) ? 1 : 0;
    _delay_us(50);
    
    return bit;
}

// Write a byte to the OneWire bus 
void onewire_writebyte(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        onewire_writebit(byte & 0x01); // Send LSB first
        byte >>= 1;
    }
}

// Read a byte from the OneWire bus
uint8_t onewire_readbyte() {
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        byte >>= 1;
        if (onewire_readbit()) {
            byte |= 0x80;  // Shift in the bit
        }
    }
    return byte;
}

// DS1820 Functions 

// Read temperature from DS1820
ds1820_result ds1820_readtemp() {
    ds1820_result result;
    if (!onewire_rst()) {
        result.lsb = 0xF1;   // Error: Sensor not found
    }

    onewire_writebyte(SKIP_ROM); 
    onewire_writebyte(START_CONV); 

    // Wait for conversion (DS1820 requires up to 750ms)
    _delay_ms(750); 

    if (!onewire_rst()) {
        result.lsb = 0xF1;  // Error: Sensor not found
    }

    onewire_writebyte(SKIP_ROM); 
    onewire_writebyte(READ_SCRATCH_PAD); 

    result.lsb = onewire_readbyte();
    result.msb = onewire_readbyte();

    return result;
}

float convert_temp(uint8_t msb, uint8_t lsb)
{
    int16_t rawTemperature = ((int16_t)msb << 8) | lsb;
    // DS1820 uses 0.5°C resolution
    return rawTemperature / 2.0;
}

void ds1820_pwron(){
        DDRB |= (1<<PINB1);
        PORTB |= (1<<PINB1);
}


void ds1820_pwroff(){
        DDRB &= ~(1<<PINB1);
        PORTB &= ~(1<<PINB1);
}
