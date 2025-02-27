#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL

int main(void)
{
	DDRD |= (1<<PD7);
	
	//set in fast pwm mode WGM21 -> 1 WGM20 -> 1
	TCCR2 |= ((1<<WGM21) | (1<<WGM20));
	
	//set output compare mode inverting mode
	//COM21 -> 1, COM20 -> 1
	TCCR2 |= ((1<<COM21)|(1<<COM20));
	
	//set no prescalar
	//CS22 ->0, CS21-> 0, CS20 -> 1
	TCCR2 |= (1<<CS20);
	TCCR2 &= ~((1<<CS22)|(1<<CS21));
	/*
	for 70% output ocr2 = 255*0.7 = 178.5 = 179
	for 80% output ocr2 = 255*0.8 = 204
	in 50% duty cycle 5V * 0.5 = 2.5V
	in 70% duty cycle 5V * 0.7 = 3.5V
	in 80% duty cycle 5V * 0.8 = 4.0V

	*/
	//OCR2 = 204;	
	while(1)
	{
		for(int i =0;i<255;i++)
		{
			OCR2 = i;
			_delay_ms(100);
		}

	}

	return 0;
}

 
