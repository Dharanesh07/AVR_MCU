

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>

						

#define Device_Write_address	0xD0
#define Device_Read_address		0xD1
#define TimeFormat12			0x40
#define AMPM					0x20

#define RS 4  
#define RW 5  
#define ENABLE 6  

//lcd function declarations
void lcd_string_data(char *ch);
void lcd_send_data(unsigned char data);
void lcd_send_cmd(unsigned char cmd);
void lcd_init(void);

//i2c function declarations
void i2c_init(void);	
void i2c_start(char write_address);
void i2c_repeated_start(char read_address);
void i2c_stop(void);
void i2c_write(char data);
char i2c_read_ack(void);	
char i2c_read_nack(void);	
bool IsItPM(char hour);

//rtc function declarations
void rtc_read_clock(char read_clock_address);
void rtc_read_calendar(char read_calendar_address);
void rtc_clock_write(char hour, char minute, char second, char AM);
void rtc_calendar_write(char day, char date, char month, char year);


void i2c_init(void)											
{
	// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
	// 100Khz = (8*10^6)/(16+2(8).4^1)
	TWBR = 0X08;
	TWSR = 0X00;	//set prescalar as 1						
}	


void i2c_start(char write_address)						
{
	
	//clear timer interrupt flag by setting TWINT to 1
	//TWEN -> i2c enable register
	//TWSTA -> i2c start condition								
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);				
	while (!(TWCR & (1<<TWINT)));	//wait till start condition is transmitted					
	TWDR = write_address;								
	TWCR = (1<<TWEN)|(1<<TWINT);							
	while (!(TWCR & (1<<TWINT)));						
										
}

void i2c_repeated_start(char read_address)				
{
										
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);				
	while (!(TWCR & (1<<TWINT)));		//wait till start condition is transmitted				
	TWDR = read_address;								
	TWCR = (1<<TWEN)|(1<<TWINT);						
	while (!(TWCR & (1<<TWINT)));						
										
}

void i2c_stop(void)											
{
	//TWSTO -> i2c stop condition
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);				
	while(TWCR & (1<<TWSTO));							 
}


void i2c_write(char data)								
{
											
	TWDR = data;										
	TWCR = (1<<TWEN)|(1<<TWINT);						
	while (!(TWCR & (1<<TWINT)));	//wait till complete data is transmitted					
	
											
}

char i2c_read_ack(void)										
{
	//TWEA -> acknowledgement bit
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);				
	while (!(TWCR & (1<<TWINT)));						
	return TWDR;										
}	

char i2c_read_nack(void)									
{
	TWCR=(1<<TWEN)|(1<<TWINT);							
	while (!(TWCR & (1<<TWINT)));						
	return TWDR;										
}	

int second,minute,hour=14,day,date,month,year;

bool IsItPM(char hour)
{
	if(hour & (AMPM))
	return 1;
	else
	return 0;
}

void rtc_read_clock(char read_clock_address)
{
	i2c_start(Device_Write_address);/* Start I2C communication with RTC */
	i2c_write(read_clock_address);
	i2c_repeated_start(Device_Read_address);/* Repeated start with device read address */

	second = i2c_read_ack();
	minute = i2c_read_ack();
	hour = i2c_read_nack();	
	i2c_stop();		
}

void rtc_read_calendar(char read_calendar_address)
{
	i2c_start(Device_Write_address);
	i2c_write(read_calendar_address);
	i2c_repeated_start(Device_Read_address);

	day = i2c_read_ack();	 
	date = i2c_read_ack();	
	month = i2c_read_ack();	
	year = i2c_read_nack();	
	i2c_stop();		
}


void rtc_clock_write(char hour, char minute, char second, char AM)
{
	hour |= AMPM;
	i2c_start(Device_Write_address);/* Start I2C communication with RTC */
	i2c_write(0);		
	i2c_write(second);	
	i2c_write(minute);	
	i2c_write(hour);	
	i2c_stop();		
}


void rtc_calendar_write(char day, char date, char month, char year)
{
	i2c_start(Device_Write_address);/* Start I2C communication with RTC */
	i2c_write(3);		
	i2c_write(day);	
	i2c_write(date);	
	i2c_write(month);	
	i2c_write(year);	
	i2c_stop();		
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


int main(void)
{
	char buffer[20];
	char* days[7]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

	rtc_clock_write(0x04,0x20,0x30,AMPM);
	rtc_calendar_write(0x07,0x26,0x12,0x20);

	i2c_init();		
	lcd_init();		

    while(1)
    {
		lcd_send_cmd(0x80);		//For displaying the characters on the 1st line
	    rtc_read_clock(0x00);
		if (hour & TimeFormat12)	
		{
			sprintf(buffer, "%02x:%02x:%02x  ", (hour & 0b00011111), minute, second);
			if(IsItPM(hour))
			strcat(buffer, "PM");
			else
			strcat(buffer, "AM");
			lcd_string_data(buffer);
		}
		
		else
		{
			sprintf(buffer, "%02x:%02x:%02x  ", (hour & 0b00011111), minute, second);
			lcd_string_data(buffer);
		}
		
		
		
		rtc_read_calendar(3);
		lcd_send_cmd(0xC0);		//For displaying the characters on the 2nd line
		sprintf(buffer, "%02x/%02x/%02x %3s ", date, month, year,days[day-1]);
		lcd_string_data(buffer);
    }
}