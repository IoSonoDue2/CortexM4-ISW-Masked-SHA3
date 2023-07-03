//ISW scheme 5 shares with randomness

#include"SecureGates.h"

void secureAND (uint64_t a_s[] , uint64_t b_s[], uint64_t c_s[]){


    uint64_t shuffled[N_SHARE];
    uint64_t z[N_SHARE][N_SHARE];

    int i,j,k;
    
    //shareCalc(a,a_s);
    //shareCalc(b,b_s);
    shuffle_array(shuffled);

    for(k=0;k<N_SHARE;k++){
        i=shuffled[k];
        for(j=i+1;j<N_SHARE;j++){
            z[i][j]=Rand64();
            z[j][i]=(z[i][j]^(a_s[i]&b_s[j]))^(a_s[j]&b_s[i]);

        }
        
    }
    shuffle_array(shuffled);
    for(i=0;i<N_SHARE;i++){
        
        c_s[i]=a_s[i]&b_s[i];
        for(k=0;k<N_SHARE;k++){
            j=shuffled[k];
            if(i!=j)
                c_s[i]=c_s[i]^z[i][j];

        }
        
    }

}
