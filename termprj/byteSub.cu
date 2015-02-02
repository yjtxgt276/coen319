#include "byteSub.h"

__device__ void byteSub(uint8_t* block){
//Nbyte=128/8=16, number of bytes in a block
    for(int i=0;i<Nrow;i++)
	for(int j=0;j<Ncol;j++){
	    block[i*Ncol+j]=d_sbox[block[i*Ncol+j]];
//	    printf("%x\t",block[i*Ncol+j]);
	}
 //   printf("Hello from GPU byteSub\n");
}

__device__ void invByteSub(uint8_t* block){
//Nbyte=128/8=16, number of bytes in a block
    for(int i=0;i<Nrow;i++)
	for(int j=0;j<Ncol;j++)
	    block[i*Ncol+j]=d_inv_sbox[block[i*Ncol+j]];
    //printf("Hello from GPU byteSub\n");
}
