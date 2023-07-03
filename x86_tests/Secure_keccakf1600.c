#include "Secure_keccakf1600.h"
#include "common.h"
#include <stdint.h>
#define NROUNDS 24


static const uint64_t KeccakF_RoundConstants[NROUNDS] = 
{
    (uint64_t)0x0000000000000001ULL,
    (uint64_t)0x0000000000008082ULL,
    (uint64_t)0x800000000000808aULL,
    (uint64_t)0x8000000080008000ULL,
    (uint64_t)0x000000000000808bULL,
    (uint64_t)0x0000000080000001ULL,
    (uint64_t)0x8000000080008081ULL,
    (uint64_t)0x8000000000008009ULL,
    (uint64_t)0x000000000000008aULL,
    (uint64_t)0x0000000000000088ULL,
    (uint64_t)0x0000000080008009ULL,
    (uint64_t)0x000000008000000aULL,
    (uint64_t)0x000000008000808bULL,
    (uint64_t)0x800000000000008bULL,
    (uint64_t)0x8000000000008089ULL,
    (uint64_t)0x8000000000008003ULL,
    (uint64_t)0x8000000000008002ULL,
    (uint64_t)0x8000000000000080ULL,
    (uint64_t)0x000000000000800aULL,
    (uint64_t)0x800000008000000aULL,
    (uint64_t)0x8000000080008081ULL,
    (uint64_t)0x8000000000008080ULL,
    (uint64_t)0x0000000080000001ULL,
    (uint64_t)0x8000000080008008ULL
};

static uint64_t load64(const unsigned char *x)
{
  unsigned long long r = 0, i;

  for (i = 0; i < 8; ++i) {
    r |= (unsigned long long)x[i] << 8 * i; //little endian
  }
  return r;
}

static void store64(uint8_t *x, uint64_t u)
{
  unsigned int i;

  for(i=0; i<8; ++i) {
    x[i] = u;
    u >>= 8;
  }
}

void KeccakF1600_StateExtractBytes(uint64_t state[][N_SHARE], unsigned char *data, unsigned int __attribute__ ((unused)) offset, unsigned int length)
{
    unsigned int i;

    for(i=0;i<(length>>3);i++)
    {
      store64(data+8*i, unshareCalc(state[i]));
    }
}

void KeccakF1600_StateXORBytes(uint64_t state[][N_SHARE], const unsigned char *data,unsigned int __attribute__ ((unused)) offset ,unsigned int length)
{
    unsigned int i;
    uint64_t data_s[length/8][N_SHARE];

    for (i = 0; i < length / 8; ++i) {

       shareCalc(load64(data + 8 * i),data_s[i]);
        //state[i] = XOR(state[i], load64(data + 8 * i));
    }
    for (i = 0; i < length / 8; ++i) {
      XOR(state[i], data_s[i],state[i]);
    }

}

void KeccakF1600_StatePermute(uint64_t state[][N_SHARE] )
{
  int round;

        uint64_t Aba[N_SHARE], Abe[N_SHARE], Abi[N_SHARE], Abo[N_SHARE], Abu[N_SHARE];
        uint64_t Aga[N_SHARE], Age[N_SHARE], Agi[N_SHARE], Ago[N_SHARE], Agu[N_SHARE];
        uint64_t Aka[N_SHARE], Ake[N_SHARE], Aki[N_SHARE], Ako[N_SHARE], Aku[N_SHARE];
        uint64_t Ama[N_SHARE], Ame[N_SHARE], Ami[N_SHARE], Amo[N_SHARE], Amu[N_SHARE];
        uint64_t Asa[N_SHARE], Ase[N_SHARE], Asi[N_SHARE], Aso[N_SHARE], Asu[N_SHARE];
        uint64_t BCa[N_SHARE], BCe[N_SHARE], BCi[N_SHARE], BCo[N_SHARE], BCu[N_SHARE];
        uint64_t Da[N_SHARE], De[N_SHARE], Di[N_SHARE], Do[N_SHARE], Du[N_SHARE];
        uint64_t Eba[N_SHARE], Ebe[N_SHARE], Ebi[N_SHARE], Ebo[N_SHARE], Ebu[N_SHARE];
        uint64_t Ega[N_SHARE], Ege[N_SHARE], Egi[N_SHARE], Ego[N_SHARE], Egu[N_SHARE];
        uint64_t Eka[N_SHARE], Eke[N_SHARE], Eki[N_SHARE], Eko[N_SHARE], Eku[N_SHARE];
        uint64_t Ema[N_SHARE], Eme[N_SHARE], Emi[N_SHARE], Emo[N_SHARE], Emu[N_SHARE];
        uint64_t Esa[N_SHARE], Ese[N_SHARE], Esi[N_SHARE], Eso[N_SHARE], Esu[N_SHARE];

        uint64_t tmp[N_SHARE];
        uint64_t max[N_SHARE];
        shareCalc(0xFFFFFFFFFFFFFFFF,max);

        //copyFromState(A, state)
        sharecpy(Aba,state[ 0]);
        sharecpy(Abe,state[ 1]);
        sharecpy(Abi,state[ 2]);
        sharecpy(Abo,state[ 3]);
        sharecpy(Abu,state[ 4]);
        sharecpy(Aga,state[ 5]);
        sharecpy(Age,state[ 6]);
        sharecpy(Agi,state[ 7]);
        sharecpy(Ago,state[ 8]);
        sharecpy(Agu,state[ 9]);
        sharecpy(Aka,state[10]);
        sharecpy(Ake,state[11]);
        sharecpy(Aki,state[12]);
        sharecpy(Ako,state[13]);
        sharecpy(Aku,state[14]);
        sharecpy(Ama,state[15]);
        sharecpy(Ame,state[16]);
        sharecpy(Ami,state[17]);
        sharecpy(Amo,state[18]);
        sharecpy(Amu,state[19]);
        sharecpy(Asa,state[20]);
        sharecpy(Ase,state[21]);
        sharecpy(Asi,state[22]);
        sharecpy(Aso,state[23]);
        sharecpy(Asu,state[24]);
        











        for( round = 0; round < NROUNDS; round += 2 )
        {
            //    prepareTheta
            XOR5(Aba,Aga,Aka,Ama,Asa,BCa);   
            XOR5(Abe,Age,Ake,Ame,Ase,BCe);
            XOR5(Abi,Agi,Aki,Ami,Asi,BCi);
            XOR5(Abo,Ago,Ako,Amo,Aso,BCo);
            XOR5(Abu,Agu,Aku,Amu,Asu,BCu);


            //thetaRhoPiChiIotaPrepareTheta(round  , A, E)
            ROL(BCe, 1,Da);
            XOR(BCu,Da,Da);
            ROL(BCi, 1,De);
            XOR(BCa,De,De);
            ROL(BCo, 1,Di);
            XOR(BCe,Di,Di);
            ROL(BCu, 1,Do);
            XOR(BCi,Do,Do);
            ROL(BCa, 1,Du);
            XOR(BCo,Du,Du);

            XOR(Aba,Da,Aba);
            sharecpy(BCa,Aba);

            XOR(Age,De,Age);
            ROL(Age, 44,BCe);
            XOR(Aki,Di,Aki);
            ROL(Aki, 43,BCi);
            XOR(Amo,Do,Amo);
            ROL(Amo, 21,BCo);
            XOR(Asu,Du,Asu);
            ROL(Asu,14,BCu);

            XOR(max,BCe,Eba);
            AND(Eba,BCi,Eba);
            XOR(BCa,Eba,Eba);

            shareCalc((uint64_t)KeccakF_RoundConstants[round],tmp);
            
            XOR(Eba,tmp,Eba);

            XOR(max,BCi,Ebe);
            AND(Ebe,BCo,Ebe);
            XOR(BCe,Ebe,Ebe);

            XOR(max,BCo,Ebi);
            AND(Ebi,BCu,Ebi);
            XOR(BCi,Ebi,Ebi);

            XOR(max,BCu,Ebo);
            AND(Ebo,BCa,Ebo);
            XOR(BCo,Ebo,Ebo);

            XOR(max,BCa,Ebu);
            AND(Ebu,BCe,Ebu);
            XOR(BCu,Ebu,Ebu);
            
            XOR(Abo,Do,Abo);
            ROL(Abo, 28,BCa);
            XOR(Agu,Du,Agu);
            ROL(Agu, 20,BCe);
            XOR(Aka,Da,Aka);
            ROL(Aka, 3,BCi);
            XOR(Ame,De,Ame);
            ROL(Ame,45,BCo);
            XOR(Asi,Di,Asi);
            ROL(Asi,61,BCu);

            XOR(max,BCe,Ega);
            AND(Ega,BCi,Ega);
            XOR(BCa,Ega,Ega);

            XOR(max,BCi,Ege);
            AND(Ege,BCo,Ege);
            XOR(BCe,Ege,Ege);

            XOR(max,BCo,Egi);
            AND(Egi,BCu,Egi);
            XOR(BCi,Egi,Egi);

            XOR(max,BCu,Ego);
            AND(Ego,BCa,Ego);
            XOR(BCo,Ego,Ego);

            XOR(max,BCa,Egu);
            AND(Egu,BCe,Egu);
            XOR(BCu,Egu,Egu);

            XOR(Abe,De,Abe);
            ROL(Abe, 1,BCa);
            XOR(Agi,Di,Agi);
            ROL(Agi, 6,BCe);
            XOR(Ako,Do,Ako);
            ROL(Ako,25,BCi);
            XOR(Amu,Du,Amu);
            ROL(Amu, 8,BCo);
            XOR(Asa,Da,Asa);
            ROL(Asa,18,BCu);

            XOR(max,BCe,Eka);
            AND(Eka,BCi,Eka);
            XOR(BCa,Eka,Eka);

            XOR(max,BCi,Eke);
            AND(Eke,BCo,Eke);
            XOR(BCe,Eke,Eke);

            XOR(max,BCo,Eki);
            AND(Eki,BCu,Eki);
            XOR(BCi,Eki,Eki);

            XOR(max,BCu,Eko);
            AND(Eko,BCa,Eko);
            XOR(BCo,Eko,Eko);

            XOR(max,BCa,Eku);  
            AND(Eku,BCe,Eku);
            XOR(BCu,Eku,Eku);

            XOR(Abu,Du,Abu);
            ROL(Abu,27,BCa);
            XOR(Aga,Da,Aga);
            ROL(Aga,36,BCe);
            XOR(Ake,De,Ake);
            ROL(Ake,10,BCi);
            XOR(Ami,Di,Ami);
            ROL(Ami,15,BCo);
            XOR(Aso,Do,Aso);
            ROL(Aso,56,BCu);

            XOR(max,BCe,Ema);
            AND(Ema,BCi,Ema);
            XOR(BCa,Ema,Ema);

            XOR(max,BCi,Eme);
            AND(Eme,BCo,Eme);
            XOR(BCe,Eme,Eme);

            XOR(max,BCo,Emi);
            AND(Emi,BCu,Emi);
            XOR(BCi,Emi,Emi);

            XOR(max,BCu,Emo);
            AND(Emo,BCa,Emo);
            XOR(BCo,Emo,Emo);

            XOR(max,BCa,Emu);
            AND(Emu,BCe,Emu);
            XOR(BCu,Emu,Emu);

            XOR(Abi,Di,Abi);
            ROL(Abi,62,BCa);
            XOR(Ago,Do,Ago);
            ROL(Ago,55,BCe);
            XOR(Aku,Du,Aku);
            ROL(Aku,39,BCi);
            XOR(Ama,Da,Ama);
            ROL(Ama,41,BCo);
            XOR(Ase,De,Ase);
            ROL(Ase, 2,BCu);

            XOR(max,BCe,Esa);
            AND(Esa,BCi,Esa);
            XOR(BCa,Esa,Esa);

            XOR(max,BCi,Ese);
            AND(Ese,BCo,Ese);
            XOR(BCe,Ese,Ese);

            XOR(max,BCo,Esi);
            AND(Esi,BCu,Esi);
            XOR(BCi,Esi,Esi);

            XOR(max,BCu,Eso);
            AND(Eso,BCa,Eso);
            XOR(BCo,Eso,Eso);

            XOR(max,BCa,Esu);
            AND(Esu,BCe,Esu);
            XOR(BCu,Esu,Esu);



            //    prepareTheta
            XOR5(Eba,Ega,Eka,Ema,Esa,BCa);
            XOR5(Ebe,Ege,Eke,Eme,Ese,BCe);
            XOR5(Ebi,Egi,Eki,Emi,Esi,BCi);
            XOR5(Ebo,Ego,Eko,Emo,Eso,BCo);
            XOR5(Ebu,Egu,Eku,Emu,Esu,BCu);

            //thetaRhoPiChiIotaPrepareTheta(round+1, E, A)

            ROL(BCe, 1, Da);
            XOR(BCu,Da,Da);

            ROL(BCi, 1, De);
            XOR(BCa,De,De);

            ROL(BCo, 1, Di);
            XOR(BCe,Di,Di);

            ROL(BCu, 1, Do);
            XOR(BCi,Do,Do);

            ROL(BCa, 1, Du);
            XOR(BCo,Du,Du);

            XOR(Eba,Da,Eba);
            sharecpy(BCa, Eba);
            XOR(Ege,De,Ege);
            ROL(Ege, 44, BCe);
            XOR(Eki,Di,Eki);
            ROL(Eki, 43, BCi);
            XOR(Emo,Do,Emo);
            ROL(Emo, 21, BCo);
            XOR(Esu,Du,Esu);
            ROL(Esu, 14, BCu);

            XOR(max,BCe,Aba);
            AND(Aba,BCi,Aba);
            XOR(BCa,Aba,Aba);

            shareCalc((uint64_t)KeccakF_RoundConstants[round+1],tmp);
            XOR(Aba,tmp,Aba);

            XOR(max,BCi,Abe);
            AND(Abe,BCo,Abe);
            XOR(BCe,Abe,Abe);

            XOR(max,BCo,Abi);
            AND(Abi,BCu,Abi);
            XOR(BCi,Abi,Abi);

            XOR(max,BCu,Abo);
            AND(Abo,BCa,Abo);
            XOR(BCo,Abo,Abo);

            XOR(max,BCa,Abu);
            AND(Abu,BCe,Abu);
            XOR(BCu,Abu,Abu);

            XOR(Ebo,Do,Ebo);
            ROL(Ebo, 28, BCa);
            XOR(Egu,Du,Egu);
            ROL(Egu, 20, BCe);
            XOR(Eka,Da,Eka);
            ROL(Eka, 3, BCi);
            XOR(Eme,De,Eme);
            ROL(Eme, 45, BCo);
            XOR(Esi,Di,Esi);
            ROL(Esi, 61, BCu);

            XOR(max,BCe,Aga);
            AND(Aga,BCi,Aga);
            XOR(BCa,Aga,Aga);

            XOR(max,BCi,Age);
            AND(Age,BCo,Age);
            XOR(BCe,Age,Age);

            XOR(max,BCo,Agi);
            AND(Agi,BCu,Agi);
            XOR(BCi,Agi,Agi);

            XOR(max,BCu,Ago);
            AND(Ago,BCa,Ago);
            XOR(BCo,Ago,Ago);

            XOR(max,BCa,Agu);
            AND(Agu,BCe,Agu);
            XOR(BCu,Agu,Agu);

            XOR(Ebe,De,Ebe);
            ROL(Ebe, 1, BCa);
            XOR(Egi,Di,Egi);
            ROL(Egi, 6, BCe);
            XOR(Eko,Do,Eko);
            ROL(Eko, 25, BCi);
            XOR(Emu,Du,Emu);
            ROL(Emu, 8, BCo);
            XOR(Esa,Da,Esa);
            ROL(Esa, 18, BCu);

            XOR(max,BCe,Aka);
            AND(Aka,BCi,Aka);
            XOR(BCa,Aka,Aka);

            XOR(max,BCi,Ake);
            AND(Ake,BCo,Ake);
            XOR(BCe,Ake,Ake);

            XOR(max,BCo,Aki);
            AND(Aki,BCu,Aki);
            XOR(BCi,Aki,Aki);

            XOR(max,BCu,Ako);
            AND(Ako,BCa,Ako);
            XOR(BCo,Ako,Ako);

            XOR(max,BCa,Aku);
            AND(Aku,BCe,Aku);
            XOR(BCu,Aku,Aku);
            
            XOR(Ebu,Du,Ebu);
            ROL(Ebu, 27, BCa);
            XOR(Ega,Da,Ega);
            ROL(Ega, 36, BCe);
            XOR(Eke,De,Eke);
            ROL(Eke, 10, BCi);
            XOR(Emi,Di,Emi);
            ROL(Emi, 15, BCo);
            XOR(Eso,Do,Eso);
            ROL(Eso, 56, BCu);

            XOR(max,BCe,Ama);
            AND(Ama,BCi,Ama);
            XOR(BCa,Ama,Ama);

            XOR(max,BCi,Ame);
            AND(Ame,BCo,Ame);
            XOR(BCe,Ame,Ame);

            XOR(max,BCo,Ami);
            AND(Ami,BCu,Ami);
            XOR(BCi,Ami,Ami);

            XOR(max,BCu,Amo);
            AND(Amo,BCa,Amo);
            XOR(BCo,Amo,Amo);

            XOR(max,BCa,Amu);
            AND(Amu,BCe,Amu);
            XOR(BCu,Amu,Amu);
            
            XOR(Ebi,Di,Ebi);
            ROL(Ebi, 62, BCa);
            XOR(Ego,Do,Ego);
            ROL(Ego, 55, BCe);
            XOR(Eku,Du,Eku);
            ROL(Eku, 39, BCi);
            XOR(Ema,Da,Ema);
            ROL(Ema, 41, BCo);
            XOR(Ese,De,Ese);
            ROL(Ese, 2, BCu);

            XOR(max,BCe,Asa);
            AND(Asa,BCi,Asa);
            XOR(BCa,Asa,Asa);

            XOR(max,BCi,Ase);
            AND(Ase,BCo,Ase);
            XOR(BCe,Ase,Ase);

            XOR(max,BCo,Asi);
            AND(Asi,BCu,Asi);
            XOR(BCi,Asi,Asi);

            XOR(max,BCu,Aso);
            AND(Aso,BCa,Aso);
            XOR(BCo,Aso,Aso);

            XOR(max,BCa,Asu);
            AND(Asu,BCe,Asu);
            XOR(BCu,Asu,Asu);


        }

        //copyToState(state, A)
        sharecpy(state[ 0],Aba);
        sharecpy(state[ 1],Abe);
        sharecpy(state[ 2],Abi);
        sharecpy(state[ 3],Abo);
        sharecpy(state[ 4],Abu);
        sharecpy(state[ 5],Aga);
        sharecpy(state[ 6],Age);
        sharecpy(state[ 7],Agi);
        sharecpy(state[ 8],Ago);
        sharecpy(state[ 9],Agu);
        sharecpy(state[10],Aka);
        sharecpy(state[11],Ake);
        sharecpy(state[12],Aki);
        sharecpy(state[13],Ako);
        sharecpy(state[14],Aku);
        sharecpy(state[15],Ama);
        sharecpy(state[16],Ame);
        sharecpy(state[17],Ami);
        sharecpy(state[18],Amo);
        sharecpy(state[19],Amu);
        sharecpy(state[20],Asa);
        sharecpy(state[21],Ase);
        sharecpy(state[22],Asi);
        sharecpy(state[23],Aso);
        sharecpy(state[24],Asu); 

        



        #undef    round
}

