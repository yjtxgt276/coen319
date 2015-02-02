#include "myAESlib.h"
#include "ioUtil.h"
#include "keySchedule.h"
#include "decrypt.h"
#include <string.h>
#include <sys/time.h>

int main(int argc, char** argv){
//timing
    struct timeval start,end;
    double exetime=0.0;
    gettimeofday(&start,NULL);
    if(argc!=3){
	printf("USAGE: %s [filename] [key]\n",argv[0]);
	return 1;
    }
    FILE* fp;
    if((fp=fopen(argv[1],"r"))==NULL){
	printf("ERR OPENING FILE %s.\n",argv[1]);
	return 1;
    }
    if(strlen(argv[2])>16){
	printf("The key should be less than 16 characters\n");
	return 1;
    }
    off_t filesize=fsize(argv[1]);
    printf("filesize: %lu bytes\n",filesize);

    char userkey[Nbyte];
    memset(userkey,0x00,Nbyte);

    strncpy(userkey,argv[2],strlen(argv[2]));
    printf("userkey:\n");
    printx(userkey,Nbyte);

    uint8_t* buffer = (uint8_t*) malloc(filesize);
    fread(buffer, filesize,1,fp);
    //printx(buffer,filesize);

    uint8_t roundkey[Nrd+1][Nrow][Ncol]; 
    memset(roundkey,0x00,(Nrd+1)*Nbyte);
    size_t rksize=(Nrd+1)*Nrow*Ncol;
    expandKey(userkey, roundkey);
    
    uint8_t *d_buffer,*d_rk;
    cudaMalloc(&d_buffer,filesize);
    cudaMemcpy(d_buffer,buffer,filesize,cudaMemcpyHostToDevice);
    //d_printx<<<1,1>>>(d_buffer,filesize);

    cudaMalloc(&d_rk,rksize);
    cudaMemcpy(d_rk,roundkey,rksize,cudaMemcpyHostToDevice);
    memset(buffer,0,filesize);

    //dim3 dimblock(1);
    //dim3 dimgrid(1);
    decrypt<<<GRIDSIZE,BLOCKSIZE>>>(d_buffer,d_rk,filesize);
    //d_printx<<<1,1>>>(d_buffer,filesize);

    cudaMemcpy(buffer, d_buffer,filesize,cudaMemcpyDeviceToHost);
    //printx(buffer,filesize);

    FILE* fpout;
    char outputname[256];
    strcpy(outputname,argv[1]);
    strcat(outputname,".plain");
    fpout = fopen(outputname,"w+");
    fwrite(buffer,filesize,1,fpout);
    free(buffer);
    //free(bufferout);
    cudaFree(d_buffer);
    cudaFree(d_rk);
    fclose(fp);
    fclose(fpout);

    gettimeofday(&end,NULL);
    exetime=(end.tv_sec*1000000+end.tv_usec-start.tv_sec*1000000-start.tv_usec)/1000000.0;
    printf("exe time: %f secs\n",exetime);
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
		//printf("thd %d reading from byte %d:\n",thd,pos);
		//printf("\tinvmixcol %d\n",round);
	    }
	}
    }
    //printf("encrypting block %d,GPU\n",pos);
}
