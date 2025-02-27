#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"
#include <avr/sleep.h>

#define TIMER2_COMPARE_VAL 31
#define TIMER2_FULL_VAL    255
#define OFF_TIME_MS        1000
#define ON_TIME_MS         100

void timer2_init();
int global_time = 0;


int main(void)
{

    DDRD |= (1<<PORTD5);
    sei();
    timer2_init();
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_mode();

    while (1)
    {
    }
    return 0;
}



void timer2_init()
{
    TCNT2 = 0x00;
    //Use clear timer on compare match

    TCCR2A |= (1<<COM2A0);
    TCCR2A &= ~(1<<COM2A1);

    TCCR2A |= (1<<WGM21);
    TCCR2A &= ~(1 << WGM20);
    TCCR2B &= ~(1 << WGM22);
    
    //TCCR2B &= ~((1 << CS21) | (1 << CS20));
    //TCCR2B |= (1 << CS22);

    //2MHz/64 = 31250Hz
    //1/31250Hz = 32uS
    //1mS / 32uS = 31.2
    //so each ISR routine will equal to 1mS

	TCCR2B |= ((1<<CS22));
	TCCR2B &= ~((1<<CS20)|(1<<CS21));

    //set interrupt mask register
	TIMSK2 |= (1<<OCIE2A);
    
    //set interrupt flag
    TIFR2 |= (1<<OCF2A);

    //output compare register
    OCR2A = TIMER2_COMPARE_VAL; 
}



ISR(TIMER2_COMPA_vect) {
    
    global_time += 1;
    if(global_time < ON_TIME_MS)
    {
        PORTD |= (1<<PORTD5);
    }
    else if(global_time < OFF_TIME_MS)
    {
        PORTD &= ~(1<<PORTD5);
    }
    else
    {
        global_time = 0;

    }

}




