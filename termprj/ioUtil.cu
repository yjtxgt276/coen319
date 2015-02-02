#include "ioUtil.h"

off_t fsize(char* filename){
    struct stat st;
    if(stat(filename,&st)==0)
	return st.st_size;
    else 
	return -1;
}

__global__ void d_printx(void* data,size_t datasize){ 
    printf("on device:\n");
    for(int i=0;i<datasize;i++){
	printf("%x", *( (uint8_t*)data + i) );
    }
    printf("\n");
}
void printx(void* data,size_t datasize){ 
    printf("on host:\n");
    for(int i=0;i<datasize;i++){
	printf("%x",*((uint8_t*)data+i));
    }
    printf("\n");
}

