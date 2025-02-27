#include <inttypes.h>
#include <avr/io.h>
#include "usart.h"
#include <util/delay.h>


typedef enum {
	V_SOLAR = 0b0000,
	V_DIODE = 0b0001,
    V_NTC   = 0b0010,
    V_LIGHT = 0b0100,
    VCC     = 0b1110,
    V_GND   = 0b1111 
}CHANNEL;


//Initilize ADC
void adc_init(void);

//De-Initilize ADC
void adc_deinit(void);

//Get Voltage (mV)
float get_voltage(CHANNEL channel);

void bp103_pwron(void);

void bp103_pwroff(void);

void adc_enable(void);
