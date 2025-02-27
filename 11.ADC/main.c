#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "adc.h"

uint16_t adc_value;

int main(void)
{
    float v_solar, solar_current;
    adc_init();
    sei();
    uart_init();
    
    while (1)
    {
        v_solar = get_voltage(V_SOLAR);
        printf("solar voltage: %f mV\n",v_solar);
        _delay_ms(1000);
        solar_current = get_solar_current();
        printf("solar current: %f mA\n",solar_current);
        _delay_ms(1000);
    }
    return 0;
}

