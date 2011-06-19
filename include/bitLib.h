/** \file bitLib.h
 * \brief Contains BIT manipulation macros
 *
 */

#ifndef BITLIB_H
#define BITLIB_H

#ifndef cbi
#define cbi(sfr, bit)     (_SFR_BYTE(sfr) &= ~_BV(bit)) 
#endif
#ifndef sbi
#define sbi(sfr, bit)     (_SFR_BYTE(sfr) |= _BV(bit))  
#endif

#define DDR_IN	0
#define DDR_OUT 1


#define HI(x) ((RF_PORT) |= (1<<(x)))
#define LO(x) ((RF_PORT) &= ~(1<<(x)))
#define WAIT_NIRQ_LOW() while(RF_PIN&(1<<NIRQ))
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

extern int isSet(unsigned int data,unsigned char bit);


#endif

