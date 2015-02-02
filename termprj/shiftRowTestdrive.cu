#include "shiftRow.h"

__global__ void wrapper(uint8_t* block);
int main(){
// try copy 2d to dev using cudamemcpy, works
    uint8_t data[Nrow][Ncol] = {0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3}; 
    uint8_t* d_data;
    uint8_t* result;
    size_t size = Nrow*Ncol*sizeof(uint8_t);
    result =( uint8_t*) malloc(size);
    cudaMalloc(&d_data, size);
    cudaMemcpy(d_data,data,size,cudaMemcpyHostToDevice);
    wrapper<<<1,1>>>(d_data);
    cudaMemcpy(result, d_data, size, cudaMemcpyDeviceToHost);
    for(int i=0;i<Nrow;i++){
	for(int j=0;j<Ncol;j++)
	    printf("%x\t",*(result+i*Nrow+j));
	printf("\n");
    }
    free(result);
    cudaFree(d_data);
    return 0;
}
__global__ void wrapper(uint8_t* block){
   shiftRow(block); 
   invShiftRow(block);
}
