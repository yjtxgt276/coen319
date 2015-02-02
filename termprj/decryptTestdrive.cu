#include "myAESlib.h"
#include "ioUtil.h"
#include "keySchedule.h"
#include "decrypt.h"
#include <string.h>

int main(int argc, char** argv){
    if(argc!=3){
	printf("USAGE: %s [filename] [key]\n",argv[0]);
	return 1;
    }
    FILE* fp;
    if((fp=fopen(argv[1],"r"))==NULL)
	printf("ERR OPENING FILE %s.\n",argv[1]);
    off_t filesize=fsize(argv[1]);
    printf("filesize=%lu\n",filesize);
    uint8_t* buffer = (uint8_t*) malloc(filesize);

    fread(buffer, filesize,1,fp);
    printx(buffer,filesize);
    uint8_t roundkey[Nrd+1][Nrow][Ncol]; 
    size_t rksize=(Nrd+1)*Nrow*Ncol;
    expandKey(argv[2], roundkey);
    
    uint8_t *d_buffer,*d_rk;
    cudaMalloc(&d_buffer,filesize);
    cudaMemcpy(d_buffer,buffer,filesize,cudaMemcpyHostToDevice);
    d_printx<<<1,1>>>(d_buffer,filesize);
    cudaMalloc(&d_rk,rksize);
    cudaMemcpy(d_rk,roundkey,rksize,cudaMemcpyHostToDevice);
    memset(buffer,0,filesize);

    //dim3 dimblock(1);
    //dim3 dimgrid(1);
    decrypt<<<GRIDSIZE,BLOCKSIZE>>>(d_buffer,d_rk,filesize);
    d_printx<<<1,1>>>(d_buffer,filesize);
    cudaMemcpy(buffer, d_buffer,filesize,cudaMemcpyDeviceToHost);
    printx(buffer,filesize);

    FILE* fpout;
    fpout = fopen(argv[2],"w+");
    fwrite(buffer,filesize,1,fpout);
    free(buffer);
    //free(bufferout);
    cudaFree(d_buffer);
    cudaFree(d_rk);
    fclose(fp);
    fclose(fpout);
    return 0;
}

__global__ void decrypt(uint8_t* d_buffer, uint8_t* roundkey,off_t filesize){
    uint8_t *rdk, *dataptr;
    int thd = blockDim.x*blockIdx.x+threadIdx.x;
    int round =0;
    for(int pos=thd*Nbyte;pos<filesize;pos+=GRIDSIZE*BLOCKSIZE*Nbyte){
	    dataptr=d_buffer+pos; 	
	for(round=0;round<Nrd+1;round++){
	    rdk=roundkey+(Nrd-round)*Nbyte;
	    if(round == 10)
		addRoundKey(dataptr,rdk);
	    else if(round == 0){
		addRoundKey(dataptr,rdk);
		invShiftRow(dataptr);
		invByteSub(dataptr);
	    }
	    else {
		addRoundKey(dataptr,rdk);
		invMixCol(dataptr);
		invShiftRow(dataptr);
		invByteSub(dataptr);
		printf("thd %d reading from byte %d:\n",thd,pos);
		printf("\tinvmixcol %d\n",round);
	    }
	}
    }
    //printf("encrypting block %d,GPU\n",pos);
}
