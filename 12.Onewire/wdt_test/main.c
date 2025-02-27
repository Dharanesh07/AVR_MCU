#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
// Function to initialize the Watchdog Timer
void watchdog_init(void) {
    cli(); // Disable global interrupts

    wdt_reset(); // Reset the Watchdog Timer to avoid unintended resets

    // Enable WDT configuration
    WDTCSR = (1 << WDCE) | (1 << WDE);  // Enable configuration mode
    // Set WDT interrupt mode with an 8-second timeout
    WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0);

    sei(); // Enable global interrupts
}
int main(void) {
    // Configure PB5 (Arduino's built-in LED on digital pin 13) as output
    DDRD |= (1 << PD5); // Set PB5 as output
    PORTD |= (1 << PD5); // Ensure LED is off initially

    // Disable unused peripherals to save power
    power_spi_disable();
    power_twi_disable();
    power_timer0_disable();
    power_timer1_disable();

    // Initialize Watchdog Timer
    watchdog_init();

    // Configure the MCU to enter power-down mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    while (1) {
        // Enter sleep mode
        cli();               // Disable interrupts
        sleep_enable();      // Enable sleep mode
        sei();               // Re-enable interrupts
        sleep_cpu();         // Put MCU to sleep
        sleep_disable();     // Disable sleep after waking up

        // MCU wakes up here after WDT interrupt
    }

    return 0;
}

// Watchdog Timer Interrupt Service Routine (ISR)
ISR(WDT_vect) {
    // Toggle PB5 to blink the LED
    PORTD ^= (1 << PD5);
}
