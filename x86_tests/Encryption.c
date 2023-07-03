
#include <stdint.h>
#include <string.h>
#include "Secure_keccakf1600.h"
#include "Encryption.h"




static void keccak_absorb(uint64_t s[][N_SHARE],
                          unsigned int r,
                          const unsigned char *m, unsigned long long int mlen,
                          unsigned char p)
{
  unsigned long long i;
  unsigned char t[200];

  while (mlen >= r)
  {
    KeccakF1600_StateXORBytes(s, m, 0, r);
    KeccakF1600_StatePermute(s);
    mlen -= r;
    m += r;

  }

  for (i = 0; i < r; ++i)
    t[i] = 0;
  for (i = 0; i < mlen; ++i) 
    t[i] = m[i];
  t[i] = p;
  t[r - 1] |= 128;

  KeccakF1600_StateXORBytes(s, t, 0, r);
}

static void keccak_squeezeblocks(unsigned char *h, unsigned long long int nblocks,
                                 uint64_t s[][N_SHARE],
                                 unsigned int r)
{
  while(nblocks > 0)
  {
    KeccakF1600_StatePermute(s);
    KeccakF1600_StateExtractBytes(s, h, 0, r);
    h += r;
    nblocks--;
  }
}


void sha3_256(unsigned char *output, const unsigned char *input,  unsigned long long inlen)
{
  uint64_t s[25][N_SHARE];

 
  //SHA3 rate =136 
  unsigned char t[SHA3_256_RATE];
  size_t i;

  for(i=0;i<25;i++)
    shareCalc(0,s[i]);
    
  /* Absorb input */
  keccak_absorb(s, SHA3_256_RATE, input, inlen, 0x06); //sha256 c=2*256 r(bit)=1600-c=1088 --> c(byte)=1088/8=136


  /* Squeeze output */
  keccak_squeezeblocks(t, 1, s, SHA3_256_RATE);

  for(i=0;i<32;i++)
      output[i] = t[i];
}

void sha3_512(unsigned char *output, const unsigned char *input,  unsigned long long inlen)
{
  uint64_t s[25][N_SHARE];
  
  unsigned char t[SHA3_512_RATE];
  size_t i;

  for(i=0;i<25;i++)
    shareCalc(0,s[i]);

  /* Absorb input */
  keccak_absorb(s, SHA3_512_RATE, input, inlen, 0x06);

  /* Squeeze output */
  keccak_squeezeblocks(t, 1, s, SHA3_512_RATE);

  for(i=0;i<64;i++)
      output[i] = t[i];
}

void shake128(unsigned char *output, unsigned long long outlen, const unsigned char *input,  unsigned long long inlen)
{
  uint64_t s[25][N_SHARE] = {0};
  unsigned char t[SHAKE128_RATE];
  unsigned long long nblocks = outlen/SHAKE128_RATE;
  size_t i;

  for(i=0;i<25;i++)
    shareCalc(0,s[i]);


  /* Absorb input */
  keccak_absorb(s, SHAKE128_RATE, input, inlen, 0x1F);

  /* Squeeze output */
  keccak_squeezeblocks(output, nblocks, s, SHAKE128_RATE);

  output += nblocks*SHAKE128_RATE;
  outlen -= nblocks*SHAKE128_RATE;

  if (outlen)
  {
    keccak_squeezeblocks(t, 1, s, SHAKE128_RATE);
    for (i = 0; i < outlen; i++)
      output[i] = t[i];
  }
}


void shake256(unsigned char *output, unsigned long long outlen,
              const unsigned char *input,  unsigned long long inlen)
{
  uint64_t s[25][N_SHARE] = {0};
  unsigned char t[SHAKE256_RATE];
  unsigned long long nblocks = outlen/SHAKE256_RATE;
  size_t i;

  for(i=0;i<25;i++)
    shareCalc(0,s[i]);


  /* Absorb input */
  keccak_absorb(s, SHAKE256_RATE, input, inlen, 0x1F);

  /* Squeeze output */
  keccak_squeezeblocks(output, nblocks, s, SHAKE256_RATE);

  output+=nblocks*SHAKE256_RATE;
  outlen-=nblocks*SHAKE256_RATE;

  if(outlen)
  {
    keccak_squeezeblocks(t, 1, s, SHAKE256_RATE);
    for(i=0;i<outlen;i++)
      output[i] = t[i];
  }
}


