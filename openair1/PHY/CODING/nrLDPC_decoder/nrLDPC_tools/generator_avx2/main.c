#include <stdio.h>
#include <immintrin.h>
#include "../../nrLDPC_types.h"
#include "../../nrLDPC_init.h"
#include "../../nrLDPC_bnProc.h"
#define NB_Z  51
void nrLDPC_cnProc_BG1_generator_AVX2(uint16_t,int);
void nrLDPC_cnProc_BG2_generator_AVX2(uint16_t,int);
int main()
{
 uint16_t Z[NB_Z]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,18,20,22,24,26,28,30,32,36,40,44,48,52,56,60,64,72,80,88,96,104,112,120,128,144,160,176,192,208,224,240,256,288,320,352,384};


	for(int i=0; i<NB_Z;i++){

	  nrLDPC_cnProc_BG1_generator_AVX2(Z[i],0);
	  nrLDPC_cnProc_BG2_generator_AVX2(Z[i],0);

}
  return(0);
}
