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


void pin_setup(void);		//initialize the pins
int key_check();
void seven_seg(int val);

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
	DDRD &= ~((1<<SW1)|(1<<SW2)|(1<<SW3)|(1<<SW4));         //set as input
	DDRB |= (0xFF); 					//set port b as output for seven segment
	DDRA |= ((1<<SEG1)|(1<<SEG2)|(1<<SEG3)|(1<<SEG4));

}


int key_check(void)
{
	//switch 1 
	if(bit_is_clear(PIND,SW1))
	{
		return 0;
	}
	//switch 2
	else if(bit_is_clear(PIND,SW2))
	{

		return 1;
	}
	//switch 3
	else if(bit_is_clear(PIND,SW3))
	{
		return 2;
	}
	//switch 4
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
		PORTB |= (0b01111110);	//value for 0
		break;
	case 1:
		PORTB = 0x00;
		SEG_PORT |= (1<<SEG1);
		PORTB |= (0b00001100);	//value for 1
		break;
	case 2:
		PORTB = 0x00;
		SEG_PORT |= (1<<SEG1);
		PORTB |= (0b10110110); //value for 2
		break;
	case 3:
		PORTB = 0x00;
		SEG_PORT |= (1<<SEG1);
		PORTB |= (0b10011110); //value for 3
		break;
	}

}
