#include "Rotate.h"

void rotate (uint64_t a[] ,int offset, uint64_t c_s[]){

    uint64_t a_s[N_SHARE];
    uint64_t b_s[N_SHARE];


    for(int i=0;i<N_SHARE;i++){
        a_s[i]=a[i] << offset;
        b_s[i]=a[i] >> (64-offset);
    }

    secureXOR(a_s,b_s,c_s);

    



}