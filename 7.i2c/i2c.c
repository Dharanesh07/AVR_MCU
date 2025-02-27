
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL         //frequency 8 Mhz



void i2c_master_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write_address(uint8_t address);
void i2c_read_address(uint8_t add);
void i2c_write_data(uint8_t data);
uint8_t i2c_read_data(void);
uint8_t i2c_read_data_ack(void);
void i2cwaitforcomplete(void);
void i2c_check_status(uint8_t value);
void i2c_slave_address_set(uint8_t ad);



int main(void)
{
	i2c_master_init();
	while(1)
	{

	}
	return 0;
}

void i2c_master_init(void)
{
	// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
	// 100Khz = (8*10^6)/(16+2(8).4^1)
	TWBR |= (0x08);			//set TWBR as 8
	TWSR &= ~((1<<TWPS1)|(1<<TWPS0));//set prescalar as 1	
}

void i2c_start(void)
{

	//clear timer interrupt flag by setting TWINT to 1
	TWCR |= (1<<TWINT);
	//TWEN -> i2c enable register
	//TWSTA -> i2c start condition
	TWCR |= ((1<<TWEN)|(1<<TWSTA));
	i2cwaitforcomplete();		//wait till start condition is transmitted
	i2c_check_status(0x08);		
}
void i2c_stop(void)
{
	//clear timer interrupt flag by setting TWINT to 1
	TWCR |= (1<<TWINT);
	//TWSTO -> i2c stop condition
	TWCR |= ((1<<TWEN)|(1<<TWSTO));
	while(!(TWCR & (1<<TWSTO))); 	// Wait till stop condition is transmitted
}

void i2c_write_address(uint8_t address)
{
	TWDR = address;
	TWCR |= ((1<<TWINT)|(1<<TWEN)); //clear i2c interrupt and enable i2c
	i2cwaitforcomplete();		//wait till complete adress is transmitted
	i2c_check_status(0x18);
}

void i2c_write_data(uint8_t data)
{
	TWDR = data;
	TWCR |= ((1<<TWINT)|(1<<TWEN)); //clear i2c interrupt and enable i2c
	i2cwaitforcomplete();		//wait till complete data is transmitted
	i2c_check_status(0x28);
}

void i2c_read_address(uint8_t add)
{
	TWDR = add;
	TWCR |= ((1<<TWINT)|(1<<TWEN)); //clear i2c interrupt and enable i2c
	i2cwaitforcomplete();		
	i2c_check_status(0x40);

}

uint8_t i2c_read_data(void)
{
	uint8_t buff;
	TWCR |= ((1<<TWINT)|(1<<TWEN)); //clear i2c interrupt and enable i2c
	i2cwaitforcomplete();		
	i2c_check_status(0x58);
	buff = TWDR;
	return buff;
}

uint8_t i2c_read_data_ack(void)
{
	uint8_t buff;
	//TWEA -> acknowledgement bit
	TWCR |= ((1<<TWINT)|(1<<TWEN)|(1<<TWEA)); 
	i2cwaitforcomplete();		
	i2c_check_status(0x50);
	buff = TWDR;
	return buff;
}

void i2c_repeated_start(void)
{
	//clear timer interrupt flag by setting TWINT to 1
	TWCR |= (1<<TWINT);
	TWCR |= ((1<<TWEN)|(1<<TWSTA));
	i2cwaitforcomplete();		//wait till start condition is transmitted
	i2c_check_status(0x10);		
	
}

void i2c_slave_address_set(uint8_t ad)
{
	TWAR |= (ad);
}

void i2cwaitforcomplete(void)
{
	//checks for the TWINT to set
	while(!(TWCR & (1<<TWINT)));
}

void i2c_check_status(uint8_t value)
{
	//TWSR -> status register outputs the values to check values
	//0xF8 = 0b11111000 last 3 bits are masked
       //status code refer pg:184,187 in datasheet	
	while((TWSR & 0xF8)!= value);
}

