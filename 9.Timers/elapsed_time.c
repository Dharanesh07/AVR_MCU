#include <avr/io.h>
#include "usart.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#define TIMER1_COMPARE_VAL 31

int timer_count=0;

void timer_init();
void start_timer();
void stop_timer();

int main()
{

    float start_time, stop_time; 
    float elapsed_time_ms; 

    float base = 2.0;
    float exponent = 7.5;
    float number = 5.0;
    float power_result, sqrt_result; 
    
    sei();
    timer_init();
	uart_init();
   

    
    while (1)
    {
        start_time = timer_count;
        _delay_ms(1000); 

        //power_result = pow(base, exponent);
        //sqrt_result = sqrt(number);

        //power_result = pow(base, sqrt_result);
        //sqrt_result = sqrt(power_result); 
        //
        //printf("square root result: %.8f\n",sqrt_result);
        //printf("power result: %.8f\n",power_result);
        //
        stop_time = timer_count; 
        elapsed_time_ms = ((stop_time - start_time));

        printf("elapsed time: %.2f mS\n",elapsed_time_ms);
        _delay_ms(1000.0);

        
    }
}


void timer_init()
{
	//set timer1 initial start value of register to 0
	TCNT1H = 0x00;
	TCNT1L = 0x00;

	//Use Clear Timer on Compare Match (CTC) mode operation
	TCCR1A &= ~((1<<WGM10)|(1<<WGM11));
	TCCR1B &= ~((1<<WGM13));
	TCCR1B |= (1<<WGM12);

	//Set prescalar as 64
    //Oscillator freq = 2MHz
    //2MHz/64 = 31250Hz
    //1/31250Hz = 32uS
    //1mS / 32uS = 313
    //so each ISR routine will equal to 10mS

	TCCR1B &= ~((1<<CS12));
	TCCR1B |= ((1<<CS10)|(1<<CS11));

	//Set interrupt mask register for Compare A
	TIMSK1 |= (1<<OCIE1A);

	//set the interrupt flag.
	TIFR1 |= (1<<OCF1A);

	//Output Compare Register
	OCR1A = TIMER1_COMPARE_VAL;

	
}



ISR(TIMER1_COMPA_vect) {
    
    timer_count += 1;

}




