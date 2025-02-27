#include "adc.h"

#define REF       1.1
#define ADC_BITS  1024
#define SHUNT_RES 10

uint16_t adc_value;

//Initilize ADC
void adc_init(void){
    //Use internal 1.1V ref 
    ADMUX |= ((1<<REFS1)|(1<<REFS0));
    //Use right shifted mode of operation
    ADMUX &= ~(1<<ADLAR);
    //Enable ADC module
    ADCSRA |= (1<<ADEN);
    
    //Enable auto triggering 
    ADCSRA |= (1<<ADATE);
    //ADCSRA &= ~(1<<ADATE);
    
    //select internal ref
    //ADMUX |= ((1 << MUX3)|(1<<MUX2)|(1<<MUX1));
    //ADMUX &= ~(1<<MUX0);

    //Make ADC run in free running mode
    ADCSRB &= ~((1<<ADTS0)|(1<<ADTS1)|(1<<ADTS2));
    
    //Enable interrupt upon ADC conversion completion
    ADCSRA |= (1<<ADIE);
    
    //Prescalar value = 128
    ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
    
}

    
//De-Initilize ADC
void adc_deinit(void){

    //Disable ADC module
    ADCSRA &= ~(1<<ADEN);
}

//Get ADC value
float get_voltage(CHANNEL channel){

   
	// Clear the last 3 bits
    // 0XF0 = 1111 0000
	ADMUX &= 0xF0;
	
	// Select the channel by adjusting last 4 bits of ADC
    ADMUX |= (channel & 0x0F);

	// Start Conversion
	ADCSRA |= (1 << ADSC);

	// wait till conversion is complete
	//while(ADCSRA & (1<<ADSC));
	
	//adc_value = ADC;

 	//convert the ADC value to voltage in (mV) and return.
	return (((adc_value * REF)/ (ADC_BITS))*0.001);
	
}	

//Get Solar Current (mA)
float get_solar_current(void){

    float v_solar, v_diode;
    v_solar = get_voltage(V_SOLAR);
    v_diode = get_voltage(V_DIODE);

	return ((v_solar - v_diode)/10);

}


ISR(ADC_vect) {
	adc_value = ADC;
}
