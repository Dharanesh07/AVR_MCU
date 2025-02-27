#define F_CPU 8000000L 

#include <avr/io.h>
#include <util/delay.h>

int main()
{
	int present = 0;
	DDRC |= (0xFF);//declare the pins as output
	PORTC &= ~(0x00);//declare the pins low

	while(1)
	{
		PORTC |= (1<<present);
	       	_delay_ms(1000);
		PORTC &= ~(1<<present);
		present++;
		if(present > 7)present = 0;	
	}
}

