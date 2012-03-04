#ifndef encryption_h
#define encryption_h

#include <stdint.h>

#define DELTA 0x9e3779b9	
#define MX ((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (k[(p&3)^e] ^ z));

extern void blockTea(uint32_t *v, int n, uint32_t const k[4]);



#endif
