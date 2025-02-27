
#include<avr/io.h>
#include<util/delay.h>

#define RS 4 //RS=0 for command & RS=1 for Data
#define RW 5 //Read=>1& Write=>0
#define EN 6 //Enable High to low

#define F_CPU 8000000UL

         
void Lcd_data(unsigned char data);
void Lcd_command(unsigned char cmd);
void LCD_Init (void);

int main()
{
 	DDRC |= 0x1F;	//declare the row pins as input
 	DDRA |= 0xF0;	//declare the column pins as output
	LCD_Init ();
 	Lcd_command(0X80); 
	unsigned char data[3][4] = { 	{'1','2','3','4'},
					{'5','6','7','8'},
					{'9','0','*','#'}};
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
					Lcd_data(data[row][col]);
					Lcd_command(0x06);
				}
			}
		}	
	}
	return 0;	
}


void Lcd_command(unsigned char cmd)
{
	PORTB = cmd;
	PORTD |= (1<<EN); //Enable High
	_delay_ms(100);
	PORTD &= ~(1<<EN);//Enable Low
	_delay_ms(100);
	PORTD = 0x00;

}

void Lcd_data(unsigned char data)
{
	PORTB = data;	
	PORTD |= (1<<EN); //Enable High
	PORTD |= (1<<RS); //RS=1 ==> Data
	_delay_ms(100);
	PORTD &= ~(1<<EN);//Enable Low
	_delay_ms(100);
	PORTD=0x00;
}

void LCD_Init (void)		
{
	DDRD = 0X70;
        DDRB = 0XFF;
	_delay_ms(2);
        Lcd_command(0x0E);
        Lcd_command(0x38);   
        Lcd_command(0X0C);   
        Lcd_command(0x14);   
        Lcd_command(0X06);   
        Lcd_command(0X01);   
}

