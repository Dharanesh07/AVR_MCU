#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"

#define BANDGAP_VOLTAGE 3.3 // Bandgap reference voltage in volts

// Function to measure VCC using the internal bandgap reference
float measure_vcc() {
    // Select the bandgap reference as ADC input (ADC channel 0x0C)
    ADMUX = (1 << REFS0) | (1 << REFS1) | (1 << MUX3) | (1 << MUX2); // AVcc as reference, select bandgap input
    _delay_ms(2); // Allow settling time for the reference voltage

    // Start the ADC conversion
    ADCSRA |= (1 << ADSC);

    // Wait for the conversion to complete
    while (ADCSRA & (1 << ADSC));

    // Read the ADC value
    uint16_t adc_value = ADC;

    // Calculate and return VCC
    return (BANDGAP_VOLTAGE * 1024.0) / adc_value;
}

int main(void) {
    float vcc;

    // Initialize peripherals
    uart_init(); // Initialize UART for debugging
    sei();       // Enable global interrupts

    // Enable ADC
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC with prescaler of 64

    while (1) {
        // Measure and print VCC
        vcc = measure_vcc();
        printf("Microcontroller VCC: %.2f V\n", vcc);

        _delay_ms(1000); // Delay for readability
    }

    return 0;
}
