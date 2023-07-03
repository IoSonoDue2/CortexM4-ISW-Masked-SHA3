#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <assert.h>
#include <unistd.h>
#include "Encryption.h"

char* generate_random_string(int length) {
  static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  char* str = malloc((length + 1) * sizeof(char));
  if (str) {
    sleep(1);
    srand(time(NULL));
    for (int i = 0; i < length; i++) {
      str[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    str[length] = '\0';
  }
  return str;
}

void main(){
    char * rnd;
    int k;
    unsigned char output_256[32];
    unsigned char output_512[64];
    unsigned char output_shake[128];

    char output_buffer_256[256+1];
    char output_buffer_512[512+1];
     char output_buffer_shake[1024+1];

    rnd=generate_random_string(rand()%200);
    printf("%s\n",rnd);


    //sha3_256(output_256,rnd,strlen(rnd));
    //send_string(input_buffer);
    shake128(output_shake, 128, rnd,  strlen(rnd));

    for (int i = 0,k=0; i < 128; i++) {

        for (int j = 7; j >= 0; j--) {
            if (output_shake[i] & (1 << j)) {
            output_buffer_shake[k]='1';
            } else {
            output_buffer_shake[k]='0';
            }
            k++;
        }
    }
    output_buffer_shake[1024]='\0';
    printf("%s\n",output_buffer_shake);


}
