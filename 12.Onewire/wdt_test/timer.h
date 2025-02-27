#include <avr/io.h>
#include <stdint.h>

// Function Prototypes
void timer2_init(uint8_t compare_val);

void disable_unused_digital_inputs(void);

void setup_power_reduction(void);

void watchdog_init(void);