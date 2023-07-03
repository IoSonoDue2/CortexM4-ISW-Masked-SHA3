#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <stdint.h>
#define SHA3_256_RATE 136
#define SHA3_512_RATE  72
#define SHAKE128_RATE 168
#define SHAKE256_RATE 136


void sha3_256(unsigned char *output, const unsigned char *input,  unsigned long long inlen);
void sha3_512(unsigned char *output, const unsigned char *input,  unsigned long long inlen);
void shake128(unsigned char *output, unsigned long long outlen, const unsigned char *input,  unsigned long long inlen);
void shake256(unsigned char *output, unsigned long long outlen,const unsigned char *input,  unsigned long long inlen);

#endif