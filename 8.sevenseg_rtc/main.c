

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>

						

#define Device_Write_address	0xD0
#define Device_Read_address		0xD1
#define TimeFormat12			0x40
#define AMPM					0x20


#define SEG1 PA0
#define SEG2 PA1
#define SEG3 PA2
#define SEG4 PA3
#define SEG_PORT PORTA


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


int second,minute,hour=14,day,date,month,year;
int min[2],hr[2],sec[2];

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

void sevseg_disp(int num, int seg)
{
	//seg -> 0,1,2,3
	unsigned char sg[4]={0x01,0x02,0x04,0x08};
	SEG_PORT = sg[seg];
	switch (num)
	{
	case 0:
		PORTB = (0b01111110);
		break;
	case 1:
		PORTB = (0b00001100);		
		break;
	case 2:
		PORTB = (0b10110110);
		break;
	case 3:
		PORTB = (0b10011110);
		break;
	case 4:
		PORTB = (0b11001100);
		break;
	case 5:
		PORTB = (0b11011010);
		break;
	case 6:
		PORTB = (0b11111010);
		break;
	case 7:
		PORTB = (0b00001110);
		break;
	case 8:
		PORTB = (0b11111110);
		break;
	case 9:
		PORTB = (0b11011110);
		break;
	}
	_delay_ms(5);
	SEG_PORT = 0x00;
	
	if(seg == 2)
	{
		PORTB |= (1<<PB0); 
		_delay_ms(5);
	}
}
void sevseg_setup()
{
	DDRB |= (0xFF); //set port b as output for seven segment
	DDRA |= ((1<<SEG1)|(1<<SEG2)|(1<<SEG3)|(1<<SEG4));
}


uint8_t bcdtodec(uint8_t val)
{
	 return ((val / 16 * 10) + (val % 16));
}
int main(void)
{
	i2c_init();		
	sevseg_setup();

	rtc_clock_write(0x04,0x20,0x30,AMPM);
	//rtc_calendar_write(0x07,0x26,0x12,0x20);
	
    while(1)
    {
		
	    rtc_read_clock(0x00);	//seconds start address
	
		hour = (hour & 0b00011111);//in 24 hr last 6 bit represent hrs
		hour = bcdtodec(hour);
		hr[0] = hour/10;
		hr[1] = hour%10;
		//sevseg_disp(hr[0],3);
		//_delay_ms(5);
		//sevseg_disp(hr[1],2);
		//_delay_ms(5);
		minute = bcdtodec(minute);
		min[0]=minute/10;
		min[1]=minute%10;
		sevseg_disp(min[0],3);
		_delay_ms(5);
		sevseg_disp(min[1],2);
		_delay_ms(5);
		
		second = bcdtodec(second);
		sec[0]=second;
		 sec[0]=second/10;
		 sec[1]=second%10;
		sevseg_disp(sec[0],1);
		_delay_ms(5);
		sevseg_disp(sec[1],0);
		_delay_ms(5);
		
		
    }
}
