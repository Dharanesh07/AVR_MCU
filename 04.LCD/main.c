/*
   RS pin lets the tell the LCD whether it is sending commands or the data
   RS pin is set to LOW -> send commands to the LCD
   RS pin is set on HIGH -> send data/characters to the LCD

   Enable pin is set to HIGH, the LCD is processing the incoming data.
   If low it does not process the incoming data.

   R/W pin is used to force lcd to read or write mode
	high = read mode
	low = write mode	
*/

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL     //frequency 8 Mhz

#define RS 4  
#define RW 5  
#define ENABLE 6  



void lcd_string_data(char *ch);
void lcd_send_data(unsigned char data);
void lcd_send_cmd(unsigned char cmd);
void lcd_init(void);

int main(void)
{
    
    
    lcd_init();
    lcd_string_data("HelloWorld");	
	while(1)
		{
			
		}
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

