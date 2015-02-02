#include "addRoundKey.h"
__global__ void wrapper(uint8_t* block, uint8_t* rdk);

int main(){
// try copy 2d to dev using cudamemcpy, works
    uint8_t data[][Ncol] = {0x0f,0x0f,0x0f,0x0f, 0x0f,0x0f, 0x0f,0x0f, 0x0f,0x0f, 0x0f,0x0f, 0x0f,0x0f, 0x0f,0x0f  }; 
    uint8_t *d_data,*d_key;
    uint8_t* result;
    uint8_t key[][Ncol] = {0xff,0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff  }; 
    size_t size = Nrow*Ncol*sizeof(uint8_t);
    result =( uint8_t*) malloc(size);
    cudaMalloc(&d_data, size);
    cudaMalloc(&d_key, size);
    cudaMemcpy(d_data,data,size,cudaMemcpyHostToDevice);
    cudaMemcpy(d_key,key,size,cudaMemcpyHostToDevice);
    wrapper<<<1,1>>>(d_data,d_key);
    cudaMemcpy(result, d_data, size, cudaMemcpyDeviceToHost);
    for(int i=0;i<Nrow;i++){
	for(int j=0;j<Ncol;j++)
	    printf("%x\t",*(result+i*Nrow+j));
	printf("\n");
    }
    free(result);
    cudaFree(d_data);
    cudaFree(d_key);
    return 0;
}

__global__ void wrapper(uint8_t* block, uint8_t* rdk){
    addRoundKey(block,rdk);
    addRoundKey(block,rdk);
}
