/*
	Common bitwise operations and port operation library
	written by Markus Lindblom 2010

	Changelog

	2010-05-29 Created library
	2010-07	   Added custom atol function


*/

#include <ctype.h>
#include <avr/io.h>
#include "include/bitLib.h"

/** \file bitLib.c 
 * \brief Common bit operations
 *
 * Common bitwise operations and port operation library
 * written by Lindblom 2010
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

