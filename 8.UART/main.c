
#include <avr/io.h>
#include "usart.h"
#include <util/delay.h>

int main()
{
    int count =0;
	uart_init();
    while (1)
    {
    	printf("%d\n",count);
        count = count +1;
        //printf("Hello world\n");
        _delay_ms(1000.0);
        //TODO: Welcome to the TUHH energy harvesting 
    }
}
