#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL         //frequency 8 Mhz

#define SW1 PD4
#define SW2 PD5
#define SW3 PD6
#define SW4 PD7
#define SW_PORT PORTD


#define SEG1 PA0
#define SEG2 PA1
#define SEG3 PA2
#define SEG4 PA3
#define SEG_PORT PORTA


#define debounce_time 200	//debounce time in microseconds

//keypad function declarations
void pin_setup(void);
int key_detect();
void seven_seg(int val);

char buffer[2];		
int main(void)
{
	int temp;
	pin_setup();

       	while(1)
        {
		temp = key_check();
		if(temp != 4)
		{
			seven_seg(temp);
		}

        }

        return 0;
}

void pin_setup(void)
{
	DDRD &= ~((1<<SW1)|(1<<SW2)|(1<<SW3)|(1<<SW4));            //set as input
	DDRB |= (0xFF); //set port b as output for seven segment
	DDRA |= ((1<<SEG1)|(1<<SEG2)|(1<<SEG3)|(1<<SEG4));

}


int key_check(void)
{
	//col 1 scanning
	if(bit_is_clear(PIND,SW1))	//check wether col 1 is high
	{
		return 0;
	}
	//col 2 scanning
	else if(bit_is_clear(PIND,SW2))
	{

		return 1;
	}
	//col 3 scanning
	else if(bit_is_clear(PIND,SW3))
	{
		return 2;
	}
	//col 4 scanning
	else if(bit_is_clear(PIND,SW4))
	{
		return 3;
	}
	else 
	{
		return 4;
	}

}

void seven_seg(int val)
{
	
	switch (val)
	{
	case 0:
		PORTB = 0x00;
		SEG_PORT |= (1<<SEG1);
		PORTB |= (0b01111110);
		break;
	case 1:
		PORTB = 0x00;
		SEG_PORT |= (1<<SEG1);
		PORTB |= (0b00001100);		
		break;
	case 2:
		PORTB = 0x00;
		SEG_PORT |= (1<<SEG1);
		PORTB |= (0b10110110);
		break;
	case 3:
		PORTB = 0x00;
		SEG_PORT |= (1<<SEG1);
		PORTB |= (0b10011110);
		break;
	}



}
