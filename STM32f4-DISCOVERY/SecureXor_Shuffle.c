//ISW scheme 5 shares with randomness

#include "SecureGates.h"




//Compute the XOR of two numbers using the ISW scheme
void secureXOR (uint64_t a_s[],uint64_t b_s[],uint64_t c_s[]){

    uint64_t shuffled[N_SHARE];
    int i,j,k;



    // Shuffle the indices array again
    shuffle_array(shuffled);

    // Compute the shares of the XOR result by iterating over the shuffled indices array and XORing the shares of a and b, and the intermediate XOR values from the z matrix
    for(i=0;i<N_SHARE;i++){
        j=shuffled[i];
        c_s[j]=a_s[j]^b_s[j];
        
    }


}





