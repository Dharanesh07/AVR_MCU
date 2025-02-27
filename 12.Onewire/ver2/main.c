#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include <avr/eeprom.h>
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
//For 5 sec delay = 4880

#define TIMER2_COMPARE_VAL 31
#define ON_TIME_MS 9760
#define ON_TIME_BP_MS 4880


#define EEPROM_MAXTEMP_ADDR 0x01
#define EEPROM_MINTEMP_ADDR 0x03 


void disable_unused_digital_inputs() {
    // Disable digital input buffers on unused pins to reduce leakage current
   DIDR0 = 0xFF & ~(1 << ADC4D); // Disable all ADC pins
   DIDR1 = 0x03; // Disable AIN0 and AIN1 (Analog Comparator inputs)
}



void tasks();

volatile int global_time = 0;
volatile bool is_day = 1;
volatile bool wakeup_flag = 1;

float curr_temp; 
float prev_temp = 0;
float mean_temp = 0;
float min_temp = 0;
float max_temp = 0;
float bp103_light_prev = 0;
float bp103_light_next = 0;
float cap_voltage;
bool is_updated = 0;
ds1820_result ds1820; //struct declaration
uint8_t buf_msb, buf_lsb;


int main(void) {

    uart_init();
    adc_init();
    sei();
    timer2_init(TIMER2_COMPARE_VAL);
    tasks(); 
    power_spi_disable();
    power_twi_disable();
    power_timer0_disable();
    power_timer1_disable();
    disable_unused_digital_inputs();
    set_sleep_mode(SLEEP_MODE_PWR_SAVE); 
    while (1) 
    {
        if(!wakeup_flag)
        {
            _delay_ms(100);
            // Enter sleep mode
            sleep_bod_disable();
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
        //printf("started\n"); 
        adc_enable();
        ds1820_pwron();
        _delay_ms(2);
        ds1820 = ds1820_readtemp();
        curr_temp = convert_temp(ds1820.msb, ds1820.lsb);
        ds1820_pwroff();
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

            //printf("debug print: in\n");
             
            buf_msb = eeprom_read_byte((uint8_t *)EEPROM_MAXTEMP_ADDR);
            buf_lsb = eeprom_read_byte((uint8_t *)EEPROM_MAXTEMP_ADDR + 0x01);
            max_temp = convert_temp(buf_msb, buf_lsb);

            buf_msb = eeprom_read_byte((uint8_t *)EEPROM_MINTEMP_ADDR);
            buf_lsb = eeprom_read_byte((uint8_t *)EEPROM_MINTEMP_ADDR + 0x01);
            min_temp = convert_temp(buf_msb, buf_lsb);
            is_updated = 0;
        }
        */   
        cap_voltage = get_voltage(VCC);
        mean_temp = (max_temp+min_temp)/2;
        
        printf("<group1>,<%.3f>,<%.2f>,<%.2f>,<%.2f>,<%.2f>\n",cap_voltage, curr_temp, mean_temp, max_temp, min_temp);
        _delay_ms(5);
        adc_deinit();//adc disable 
}


ISR(TIMER2_COMPA_vect) {
    global_time ++;
    if(global_time == ON_TIME_BP_MS)
    {
        wakeup_flag = 1;
        adc_enable();
        bp103_pwron();
        bp103_light_next = get_voltage(V_LIGHT);
        //printf("checking every 5 sec...\t value = %f\n",bp103_light_next);
        //_delay_ms(5);
        bp103_pwroff();
        adc_deinit();
        if(bp103_light_next == 0 && bp103_light_prev == 0) 
        {
            is_day=0; // change to 0 for night detection 
            global_time = 0;
            wakeup_flag = 0; // change to 0 for sleep
        }
        else 
        {
            wakeup_flag = 1;
            is_day = 1;
        }
        bp103_light_prev = bp103_light_next;
    }

    if(global_time >= ON_TIME_MS )
    {
        if(is_day)
        {
            //sleep_disable(); 
            tasks();
            global_time = 0;
        }
    }

}

