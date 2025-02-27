/*
	In a matrix keypad rows and columns are connected together
	Here, row pins are made as input and column pins are made as output
	Each col pin is pulsed high and row pins input is checked in order to detect the key 
	press.
	This process is repeated for four column pins to detect the keypress
*/

#include<avr/io.h>
#include<util/delay.h>


#define F_CPU 8000000UL

#define RS 4  
#define RW 5  
#define ENABLE 6  

         
void lcd_string_data(char *ch);
void lcd_send_data(unsigned char data);
void lcd_send_cmd(unsigned char cmd);
void lcd_init(void);


int main()
{
 	DDRC |= 0x1F;	//declare the row pins as input
 	DDRA |= 0xF0;	//declare the column pins as output
	lcd_init();
 	lcd_send_cmd(0X80); 
	unsigned char keys[3][4] = { 	
								{'1','2','3','4'},
								{'5','6','7','8'},
								{'9','0','*','#'}
								};
	unsigned char colAddr[4] = { 0x10, 0x20, 0x40, 0x80}; // make col as high
	unsigned char rowAddr[3] = { 0x20, 0x40, 0x80};	//corresponding row values
	while(1)
	{
		for(int col=0;col<4;col++)
		{
			//or operation makes prev port also high
			PORTA = colAddr[col];
			for(int row=0;row<3;row++)
			{
				if( PINC == rowAddr[row])
				{
					lcd_send_data(keys[row][col]);
					lcd_send_cmd(0x06);
				}
			}
		}	
	}
	return 0;	
}

void lcd_string_data(char *ch)
{
	for(int i=0;ch[i]!='\0';i++)
	{
		lcd_send_data(ch[i]);       
	}

}


void lcd_send_cmd(unsigned char cmd)
{
	PORTB = cmd;
	PORTD &= ~(1<<RS); //set command mode by rs = 0 
    PORTD &= ~(1<<RW);  //set write mode by rw = 0
	PORTD |= (1<<ENABLE); //make enable high  
	_delay_ms(2);
	PORTD &= ~(1<<ENABLE);  //make enable low
	_delay_ms(2);

}

void lcd_send_data(unsigned char data)
{
	PORTB = data;	
	 PORTD &= ~(1<<RW);     //send write mode by rw = 0
	PORTD |= (1<<ENABLE);   //make enable high
	PORTD |= (1<<RS);       //set data mode by rs = 1 
	_delay_ms(2);
	PORTD &= ~(1<<ENABLE);  //make enable low
	_delay_ms(2);
}
void lcd_init (void)		
{
		DDRD |= ((1 << RS)|(1 << RW)|(1 << ENABLE));
		//DDRD = 0X70;	//set control port as output
        DDRB = 0XFF;	//set data port b as output
		_delay_ms(2);
		lcd_send_cmd(0x0E);
        lcd_send_cmd(0x38); //use two lines 8 bit mode  
		lcd_send_cmd(0X0C); //Display ON, cursor blinking 
        lcd_send_cmd(0x14); //Cursor position shifts to right  
        lcd_send_cmd(0X06);   
        lcd_send_cmd(0X01);  //clear screen 
}

