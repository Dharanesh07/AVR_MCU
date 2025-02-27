#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "timer.h"


void timer2_init(uint8_t compare_val)
{
    TCNT2 = 0x00;
    //Use clear timer on compare match

    TCCR2A |= (1<<COM2A0);
    TCCR2A &= ~(1<<COM2A1);

    TCCR2A |= (1<<WGM21);
    TCCR2A &= ~(1 << WGM20);
    TCCR2B &= ~(1 << WGM22);
   
    //set prescalar to 64
    TCCR2B &= ~((1 << CS21) | (1 << CS20));
    TCCR2B |= (1 << CS22);
   
    //set prescalar to 8
    //TCCR2B &= ~((1 << CS22) | (1 << CS20));
    //TCCR2B |= (1 << CS21);

    //set prescalar to 1024
    //TCCR2B |= ((1 << CS22)|(1 << CS21) | (1 << CS20));


    //set interrupt mask register
	TIMSK2 |= (1<<OCIE2A);
    
    //set interrupt flag
    TIFR2 |= (1<<OCF2A);

    //output compare register
    OCR2A = compare_val; 
}

// Function to initialize the Watchdog Timer
void watchdog_init(void) {
    cli(); // Disable global interrupts

    // Reset WDT to avoid unwanted resets
    wdt_reset();

    // Set up WDT interrupt mode and timeout period (1 second)
    WDTCSR &= ~(1 << WDE); // Enable configuration mode
    WDTCSR |= (1 << WDIE); // Enable configuration mode
    WDTCSR |= (1 << WDCE);
    WDTCSR &= ~((1 << WDP0) | (1 << WDP2) | (1 << WDP1)); // 1-second timeout and enable interrupt
    WDTCSR |= (1<<WDP3);

    sei(); // Enable global interrupts
}


