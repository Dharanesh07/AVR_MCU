#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <avr/power.h>

#include "usart.h"
#include "ds1820.h"
#include "timer.h"
#include "adc.h"


//2MHz/64 = 31250Hz
//1/31250Hz = 32uS
//1mS / 32uS = 31.2
//Total ticks for 1 second = 31250
//Ticks per interrupt = OCR2A+1 = 31+1 = 32
//For 1 sec delay = 31250/32 = 976
//For 10 sec delay = 9760

#define TIMER2_COMPARE_VAL 31
#define ON_TIME_MS 976

#define EEPROM_MAXTEMP_ADDR 0x01
#define EEPROM_MINTEMP_ADDR 0x03 



void tasks();

volatile int global_time = 0;
volatile bool wakeup_flag = 0;
float curr_temp; 
float prev_temp = 0;
float mean_temp = 0;
float min_temp = 0;
float max_temp = 0;
float bp103_light;
float cap_voltage;
bool is_updated = 0;
ds1820_result ds1820; //struct declaration
uint8_t buf_msb, buf_lsb;


int main(void) {

    uart_init();
    adc_init();
    sei();
    timer2_init(TIMER2_COMPARE_VAL);
    power_spi_disable();
    power_twi_disable();
    power_timer0_disable();
    power_timer1_disable();
    set_sleep_mode(SLEEP_MODE_PWR_SAVE); 
    while (1) {
        if(wakeup_flag){
            wakeup_flag = 0;
            //tasks();
            //_delay_ms(10);
            // Enter sleep mode
            cli();               // Disable interrupts
            sleep_enable();      // Enable sleep mode
            sei();               // Re-enable interrupts
            sleep_cpu();         // Put MCU to sleep
            sleep_disable();     // Disable sleep after waking up
        }
    }
    return 0;
}

void tasks()
{
        
        //sleep_disable(); // deletes the SE bit
        adc_enable();
        ds1820_pwron();
        bp103_pwron();
        ds1820 = ds1820_readtemp();
        curr_temp = convert_temp(ds1820.msb, ds1820.lsb);
        if(curr_temp > max_temp)
        {
            max_temp = curr_temp;
            /*
            eeprom_write_byte((uint8_t *)EEPROM_MAXTEMP_ADDR, ds1820.msb);
            eeprom_write_byte((uint8_t *)(EEPROM_MAXTEMP_ADDR + 0x01), ds1820.lsb);
            //prev_temp = curr_temp; 
            is_updated = 1;
        */
        }
        else if(curr_temp < min_temp)
        {
            min_temp = curr_temp;
        /*
            eeprom_write_byte((uint8_t *)EEPROM_MINTEMP_ADDR, ds1820.msb);
            eeprom_write_byte((uint8_t *)(EEPROM_MINTEMP_ADDR + 0x01), ds1820.lsb);
            //prev_temp = curr_temp; 
            is_updated = 1;
        */    
        }
        else
        {
            //prev_temp = curr_temp;

        }
       /* 
         if(is_updated){

            printf("debug print: in\n");
             
            buf_msb = eeprom_read_byte((uint8_t *)EEPROM_MAXTEMP_ADDR);
            buf_lsb = eeprom_read_byte((uint8_t *)EEPROM_MAXTEMP_ADDR + 0x01);
            max_temp = convert_temp(buf_msb, buf_lsb);

            buf_msb = eeprom_read_byte((uint8_t *)EEPROM_MINTEMP_ADDR);
            buf_lsb = eeprom_read_byte((uint8_t *)EEPROM_MINTEMP_ADDR + 0x01);
            min_temp = convert_temp(buf_msb, buf_lsb);
            is_updated = 0;
        }
       */ 
        bp103_light = get_voltage(V_LIGHT);
        cap_voltage = get_capvoltage();
        mean_temp = (max_temp+min_temp)/2;
        
    printf("<group1>,<%.3f>,<%.2f>,<%.2f>,<%.2f>,<%.2f>\n",cap_voltage, curr_temp, mean_temp, max_temp, min_temp);
     _delay_ms(10);
        //printf("Temperature: %.2f°C\n", ds1820_temp);
        bp103_pwroff();
        ds1820_pwroff();
        adc_deinit();//adc disable 
}


ISR(TIMER2_COMPA_vect) {
    global_time ++;
    if(global_time >= ON_TIME_MS)
    {
        tasks();
        global_time = 0;
        wakeup_flag = 1;
    }
}

