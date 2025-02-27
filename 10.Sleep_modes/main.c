#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "usart.h"

int main(void)
{
    uart_init();
    cli(); // deactivate interrupts
    printf("Initiating sleep mode");
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // choose power down mode
    
    sleep_enable(); // sets the SE (sleep enable) bit
    sei(); // 
    sleep_cpu(); // sleep now!!
    sleep_disable(); // deletes the SE bit
  /* 
    while (1)
    {
    }
    return 0;
*/

}
