#include "include/checksum.h"

/** \file checksum.c
 *
 * \brief CRC32 calculations
 *
 *  CCITT CRC-32 (Autodin II) polynomial: 
 *  X32+X26+X23+X22+X16+X12+X11+X10+X8+X7+X5+X4+X2+X+1 
 */



/** \brief Calculate CRC32 checksum
 * 
 * \param crc		"Starting" CRC value
 * \param buffer	The data that needs CRC'ing
 * \param length	Length of data to perform CRC on
 * 
 * \returns uint32_t	The calculated CRC32 value
 */ 
unsigned long CRC32 (unsigned long crc, char *buffer, int length) 
   { 
   int   i,j ; 
   for (i=0; i<length; i++) 
      { 
      crc = crc ^ *buffer++ ; 
      for (j=0; j<8; j++) 
         { 
         if (crc & 1) 
            crc = (crc>>1) ^ 0xEDB88320 ; 
         else 
            crc = crc >>1 ; 
         } 
      } 
   return crc ; 
   } 


