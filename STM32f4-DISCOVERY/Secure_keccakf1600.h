#ifndef SECURE_KECCAKF1600_H
#define SECURE_KECCAKF1600_H

#include <stdint.h>
#include <assert.h>
#include "Rotate.h"

#include"SecureGates.h"

#define XOR(a,b,c) (secureXOR(a,b,c))
#define XOR5(a,b,c,d,e,r) (XOR(d,e,r), XOR(c,r,r), XOR(b,r,r), XOR(a,r,r))
#define AND(a,b,c) (secureAND(a,b,c))
#define ROL(a, offset,c) rotate(a,offset,c)

/*
#define XOR(a,b) (a^b)
#define XOR5(a,b,c,d,e) (a^b^c^d^e)
#define AND(a,b) (a&b)
#define ROL(a, offset) ((a << offset)^(a >> (64-offset)))
*/

void KeccakF1600_StateExtractBytes(uint64_t state[][N_SHARE], unsigned char *data, unsigned int offset, unsigned int length);
void KeccakF1600_StateXORBytes(uint64_t state[][N_SHARE], const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakF1600_StatePermute(uint64_t  state[][N_SHARE]);

#endif