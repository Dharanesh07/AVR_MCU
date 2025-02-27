#ifndef __AVR_ATmega32A__
	#define __AVR_ATmega32A__
#endif
#define F_CPU 8000000L 

#include <avr/io.h>
#include <util/delay.h>

int main()
{
    //DDRC |=( 1<< DDC0);
    DDRC = 0b00000001;

    while(1)
    {
        //PORTC ^= (1<<PORTC0);
        PORTC = 0b00000001;
        _delay_ms(500);
        
        _delay_ms(500);
    }
    return 0;
}
