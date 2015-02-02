#include "mixCol.h"

__device__ void mixCol(uint8_t* block){
//in GF(2^8), defined by x^8+x^4+x^3+x+1
    uint8_t temp[Nrow][Ncol];
    for(int i=0;i<Nrow;i++){
	for(int j=0;j<Ncol;j++){
	    temp[i][j] = block[i*Ncol+j];
	    block[i*Ncol+j]=0;
	}
    }
    for(int i=0;i<Nrow;i++){
	for(int j=0;j<Ncol;j++){
	    for(int count=0;count<Nrow;count++)
		block[i*Ncol+j] ^= gfmultiply(mix[i][count],temp[count][j]);
	}
    }
    //printf("hello from GPU mixCol\n");
}

__device__ void invMixCol(uint8_t* block){
//in GF(2^8), defined by x^8+x^4+x^3+x+1
    uint8_t temp[Nrow][Ncol];
    for(int i=0;i<Nrow;i++){
	for(int j=0;j<Ncol;j++){
	    temp[i][j] = block[i*Ncol+j];
	    block[i*Ncol+j]=0;
	}
    }
    for(int i=0;i<Nrow;i++){
	for(int j=0;j<Ncol;j++){
	    for(int count=0;count<Nrow;count++)
		block[i*Ncol+j] ^= gfmultiply(invmix[i][count],temp[count][j]);
	}
    }
    //printf("hello from GPU mixCol\n");
}

__device__ uint8_t gfmultiply(uint8_t a, uint8_t b){
    uint8_t p=0;
    uint8_t counter;
    uint8_t carry;
    for(counter=0;counter<8;counter++){
	if(b&1)
	    p^=a;
	carry = a & 0x80;
	a<<=1;
	if(carry)
	    a^=0x1b;
	b>>=1;
    }
    return p;
}























