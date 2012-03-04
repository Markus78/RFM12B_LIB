/*
	Common bitwise operations and port operation library
*/

#include <ctype.h>
#include <avr/io.h>
#include "include/bitLib.h"

/** \file bitLib.c 
 * \brief Common bit operations
 *
 * Common bitwise operations and port operation library
 */

/** \brief Check if bit is set
 *
 * \param data uint16_t to check bit in
 * \param bit  bit number to check
 * \todo Pointless to check more than 8 bits?
 */
int isSet(unsigned int data,unsigned char bit)
{
	if  ( CHECK_BIT(data,bit) ){
		return 1;
	}
	return 0;
}

