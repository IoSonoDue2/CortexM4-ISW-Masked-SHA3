#ifndef COMMON_H
#define COMMON_H 
#include<stdlib.h>
#include<time.h>
#include <stdint.h>

#define N_SHARE 5

void shareCalc(uint64_t num,uint64_t *arr);
uint64_t unshareCalc(uint64_t arr[]);
void sharecpy(uint64_t a[], uint64_t b[]);
void shuffle_array(uint64_t arr[]);
uint64_t Rand64(void);


#endif
