#include "shiftRow.h"
__device__ void shiftRow(uint8_t* block){
    uint8_t temp[Ncol] ;
    for(int i=0;i<Nrow;i++){
	for(int j=0;j<Ncol;j++){
	    temp[j]=block[i*Ncol+j];
	    //printf("%x\t",temp[j]);
	}
	for(int j=0;j<Ncol;j++)
	    block[i*Ncol+j]=temp[(j+i)%Ncol];
	//printf("\n");
    }
    //printf("hello from GPU, shiftRow\n");
}

__device__ void invShiftRow(uint8_t* block){
    uint8_t temp[Ncol] ;
    for(int i=0;i<Nrow;i++){
	for(int j=0;j<Ncol;j++){
	    temp[j]=block[i*Ncol+j];
	}
	for(int j=0;j<Ncol;j++)
	    block[i*Ncol+j]=temp[(j+Ncol-i)%Ncol];
    }
    //printf("hello from GPU, shiftRow\n");
}
