#include "usart.h"

static int usart_putchar(char c, FILE *stream)
{
    if (c == '\n')
        usart_putchar('\r', stream);

    usart_put(c);
    return 0;
}

static int usart_getchar(FILE *stream)
{
    int data = _FDEV_EOF;
    while (data == _FDEV_EOF)
        data = usart_get();

    if (data == '\r')
        data = '\n';

    return data;
}

static FILE usart_stdout_stdin = FDEV_SETUP_STREAM(usart_putchar, usart_getchar, _FDEV_SETUP_RW);

void uart_init(void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Asynchron 8N1
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);  // UART RX einschalten
#if USE_2X
    UCSR0A |= (1 << U2X0);
#endif

    stdout = &usart_stdout_stdin;
    stdin = &usart_stdout_stdin;
}

uint8_t uart_getc(void)
{
    while (!(UCSR0A & (1 << RXC0))) // warten bis Zeichen verfuegbar
        ;
    return UDR0; // Zeichen aus UDR an Aufrufer zurueckgeben
}

int uart_putc(unsigned char c)
{
    while (!(UCSR0A & (1 << UDRE0))) /* warten bis Senden moeglich */
    {
    }

    UDR0 = c; /* sende Zeichen */
    return 0;
}

void uart_puts(char *s)
{
    while (*s)
    {
        uart_putc(*s);
        s++;
    }
}

void usart_put(unsigned char c)
{
    while (!(UCSR0A & 0x20))
        ;
    UDR0 = c;
}

int usart_get(void)
{
    if (!(UCSR0A & 0x80))
        return _FDEV_EOF;

    uint8_t status = UCSR0A;
    uint8_t data = UDR0;

    if (status & 0x1C)
        return _FDEV_ERR;

    return data;
}