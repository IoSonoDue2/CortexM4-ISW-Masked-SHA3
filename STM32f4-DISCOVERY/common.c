#include <libopencm3/stm32/rng.h>
#include "common.h"

void randombytes(unsigned char *x,unsigned long long xlen);

// Compute the shares for a given number
void shareCalc(uint64_t num,uint64_t *arr){
    // Assign random values to each share except for the last one
    // The last share is assigned the XOR of the other shares and the input number

    uint64_t shuffled[N_SHARE];

    shuffle_array(shuffled);

    arr[shuffled[N_SHARE-1]]=num;

    for(int i=0; i<N_SHARE-1;i++){
        arr[shuffled[i]]=Rand64();

        arr[shuffled[N_SHARE-1]]=arr[shuffled[N_SHARE-1]]^arr[shuffled[i]];
    }

    
}

uint64_t unshareCalc(uint64_t arr[]){
    // Compute the XOR of all the shares to get the original number
    uint64_t num=arr[0];
    for(int i=1;i<N_SHARE;i++){
        num=num^arr[i];
    }
    return num;
}

void sharecpy(uint64_t a[], uint64_t b[]){
    for(int i=0;i<N_SHARE;i++){
        a[i]=b[i];
    }
}

uint64_t Rand64(){
    
    // Generate random 64-bit integer
    uint64_t random_number;
    randombytes((unsigned char*)&random_number, sizeof(random_number));
    return random_number;
}


// Shuffle an array in-place
void shuffle_array(uint64_t arr[]) {
    srand(time(NULL));  // Inizializza il generatore di numeri casuali
    int i;
    for (i=0;i<N_SHARE;i++){
        arr[i]=i;
    }

    for (i = N_SHARE - 1; i > 0; i--) {
        int j = rand() % (i + 1);  // Sceglie un indice casuale da 0 a i
        // Scambia l'elemento i con l'elemento j
        uint64_t temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void randombytes(unsigned char *x,unsigned long long xlen)
{
    union
    {
        unsigned char aschar[4];
        uint32_t asint;
    } random;

    while (xlen > 4)
    {
        random.asint = rng_get_random_blocking();
        *x++ = random.aschar[0];
        *x++ = random.aschar[1];
        *x++ = random.aschar[2];
        *x++ = random.aschar[3];
        xlen -= 4;
    }
    if (xlen > 0)
    {
        for (random.asint = rng_get_random_blocking(); xlen > 0; --xlen)
        {
            *x++ = random.aschar[xlen - 1];
        }
    }
}