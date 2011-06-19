#include <avr/io.h>
#include "include/rs232.h"

/** \file rs232.c
 *
 * \brief ATMega328p uart library
 *
 *
 *	\todo	Add interrupt driven support
 */


/** \breif Initialize UART.
 *
 * \param baud The Baudrate ( see datasheet )
 */
void usartInit(unsigned int baud)
{
 	UBRR0H = (unsigned char)(baud>>8);
 	UBRR0L = (unsigned char)baud;
 /* enable receiver/transmitter */
 	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
}

/** \breif Transmit byte to UART
 *
 * \param data uint8_t to transfer
 */
void usartTransmit(unsigned char data)
{
 while(!(UCSR0A & (1<<UDRE0)))
  ;
  UDR0 = data;
}

/** \brief Read uint8_t from usart
 *
 * \returns uint8_t The uint8_t read from the USART
 */
char usartReadChar(void)
{
   //Wait untill a data is available
   while(!(UCSR0A & (1<<RXC0)))
   {
      //Do nothing
   }
   //Now USART has got data from host
   //and is available is buffer
   return UDR0;
}

