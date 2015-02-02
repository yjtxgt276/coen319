#include "mixCol.h"

__global__ void wrapper(uint8_t* block);
int main(){
    uint8_t data[][Ncol]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}; 
    uint8_t* d_data;
    size_t size = Ncol*Nrow*sizeof(uint8_t);
    cudaMalloc(&d_data, size);
    cudaMemcpy(d_data,data,size, cudaMemcpyHostToDevice);
    wrapper<<<1,1>>>(d_data);
    //for(int i=0)
    uint8_t* result =( uint8_t*) malloc(size);
    cudaMemcpy(result, d_data, size, cudaMemcpyDeviceToHost);
    for(int i=0;i<Nrow;i++){
	for(int j=0;j<Ncol;j++)
	    printf("%x\t",*(result+i*Nrow+j));
	printf("\n");
    }
    cudaFree(d_data);
    return 0;
}
__global__ void wrapper(uint8_t* block){
   mixCol(block); 
   invMixCol(block);
}
