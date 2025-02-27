#include "adc.h"

#define REF       3.25
#define ADC_BITS  1024.0
#define BANDGAP   1.1

#define R1_KOHM   30
#define R2_KOHM   10

//Initilize ADC
void adc_init(void){
    //Use internal 1.1V ref 
   // ADMUX |= ((1<<REFS1)|(1<<REFS0));
    

    //Use external 3.3V ref 
    ADMUX |= ((1<<REFS0));
    ADMUX &= ~(1<<REFS1);

    //Use right shifted mode of operation
    ADMUX &= ~(1<<ADLAR);
    
    //Enable ADC module
    ADCSRA |= (1<<ADEN);
    
    //Enable auto triggering 
    //ADCSRA |= (1<<ADATE);
    
    //Disable auto triggering
    ADCSRA &= ~(1<<ADATE);
    
    //select internal ref
    //ADMUX |= ((1 << MUX3)|(1<<MUX2)|(1<<MUX1));
    //ADMUX &= ~(1<<MUX0);

    //Make ADC run in free running mode
    //ADCSRB &= ~((1<<ADTS0)|(1<<ADTS1)|(1<<ADTS2));
    
    //Enable interrupt upon ADC conversion completion
    //ADCSRA |= (1<<ADIE);
    
    //Prescalar value = 128
    ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
   
    // Start the ADC Conversion
    //ADCSRA |= (1 << ADSC);
}

void adc_enable(void){

    //Enable ADC module
    ADCSRA |= (1<<ADEN);
}

//De-Initilize ADC
void adc_deinit(void){

    //Disable ADC module
    ADCSRA &= ~(1<<ADEN);
}

//Get ADC value
float get_voltage(CHANNEL channel){

    
    uint16_t adc_value;
    float temp_var; 
	
    // Clear the last 3 bits
    // 0XF0 = 1111 0000
	ADMUX &= 0xF0;
	
	// Select the channel by adjusting last 4 bits of ADC
    ADMUX |= (channel & 0x0F);
    _delay_us(10);
	
    // Start Conversion
	ADCSRA |= (1 << ADSC);

	// wait till conversion is complete
	while(ADCSRA & (1<<ADSC));
    adc_value = ADC;
	
    //printf("output adc value: %u\n",adc_value);
 	//convert the ADC value to voltage in  and return.
    if(channel == VCC)
    {
        temp_var = (ADC_BITS/adc_value)*BANDGAP;
    }
    else
    {
        temp_var = ((adc_value*REF)/ (ADC_BITS));
    }
    //printf("output adc value: %f\n",temp_var);
    return (temp_var);
	
}	


void bp103_pwron(){
        DDRD |= (1<<PIND7);
        PORTD |= (1<<PIND7);
}


void bp103_pwroff(){
        DDRD &= ~(1<<PIND7);
        PORTD &= ~(1<<PIND7);
}




/*
ISR(ADC_vect) {
    
    adc_value = ADC;

}
*/
    




