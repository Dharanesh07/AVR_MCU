#include <inttypes.h>
#include <avr/io.h>

#define BAUD 9600UL

#include <util/setbaud.h>
#include <stdio.h>



uint8_t uart_getc(void);
void uart_init(void);
int uart_putc(unsigned char c);
void uart_puts(char *s);

//used for stdio
void usart_put(unsigned char c);
int usart_get(void);