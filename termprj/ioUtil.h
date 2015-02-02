#ifndef IOUTIL_H 
#define IOUTIL_H
#include <sys/stat.h>
#include <stdio.h>
#include <cuda.h>
#include <stdint.h>
off_t fsize(char* filename);
__global__ void d_printx(void* data,size_t datasize); 
void printx(void* data,size_t datasize); 


#endif
