#ifndef RS232_H
#define RS323_H

extern void usartInit(unsigned int baud);
extern void usartTransmit(unsigned char data);
extern char usartReadChar(void);

#endif

