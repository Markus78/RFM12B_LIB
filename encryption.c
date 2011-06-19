#include "include/encryption.h"

/** \file encryption.c
 *
 * \brief Contains cryptographics methods
 */


/** \brief Encrypt using block tiny encryption algorithm.
 *
 * \param v	Data to encrypt
 * \param n	Length
 * \param k	Private key
 */
void blockTea(uint32_t *v, int n, uint32_t const k[4]) 
{
    uint32_t y, z, sum;
    unsigned p, rounds, e;
    if (n > 1) {          /* Coding Part */
      rounds = 6 + 52/n;
      sum = 0;
      z = v[n-1];
      do {
        sum += DELTA;
        e = (sum >> 2) & 3;
        for (p=0; p<n-1; p++)
          y = v[p+1], z = v[p] += MX;
        y = v[0];
        z = v[n-1] += MX;
      } while (--rounds);
    } else if (n < -1) {  /* Decoding Part */
      n = -n;
      rounds = 6 + 52/n;
      sum = rounds*DELTA;
      y = v[0];
      do {
        e = (sum >> 2) & 3;
        for (p=n-1; p>0; p--)
          z = v[p-1], y = v[p] -= MX;
        z = v[n-1];
        y = v[0] -= MX;
      } while ((sum -= DELTA) != 0);
    }
  }


