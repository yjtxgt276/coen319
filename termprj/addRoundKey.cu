#include "addRoundKey.h"

__device__ void addRoundKey(uint8_t* block, uint8_t* roundKey ){
    for(int i=0;i<Nbyte;i++)
	block[i] ^= roundKey[i];
    //printf("hello from addRoundKey, GPU\n");
}
