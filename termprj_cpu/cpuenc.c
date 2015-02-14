#include "cpuaeslib.h"
#include <string.h>
#include <sys/time.h>

void encrypt(uint8_t* buffer, uint8_t (*roundkey)[Nrow][Ncol],off_t filesize);

int main(int argc, char** argv){
//timing
    struct timeval start,end;
    double exetime=0.0;
    gettimeofday(&start,NULL);
//check input
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
    //printf("filesize: %lu bytes.\n",filesize);
//pad key
    char userkey[Nbyte];
    memset(userkey,0x00,Nbyte);
    strncpy(userkey,argv[2],strlen(argv[2]));
    //printf("userkey in hex:\n");
    //printx(userkey,Nbyte);
//setup memory
    uint8_t* buffer = (uint8_t*) malloc(filesize*sizeof(uint8_t));
    fread(buffer,sizeof(uint8_t),filesize,fp);
    //printf("in buffer:\n");
    //printx(buffer,filesize);
    uint8_t roundkey[Nrd+1][Nrow][Ncol]; 

    size_t rksize=(Nrd+1)*Nrow*Ncol;
    expandKey(userkey, roundkey);
//encrypt
    encrypt(buffer,roundkey,filesize);
//output
    //printf("main 47\n");
    FILE* fpout;
    char outputname[256];
    strcpy(outputname,argv[1]);
    strcat(outputname,".cipher");
    fpout = fopen(outputname,"w+");
    fwrite(buffer,filesize,1,fpout);
//clean up
    //printf("main 54\n");
    free(buffer);
    //printf("main 56\n");
    fclose(fp);
    fclose(fpout);
//timing

    gettimeofday(&end,NULL);
    exetime=(end.tv_sec*1000000+end.tv_usec-start.tv_sec*1000000-start.tv_usec)/1000000.0;
    printf("exe time: %f secs\n",exetime);
    return 0;
}

void encrypt(uint8_t* buffer, uint8_t (*roundkey)[Nrow][Ncol],off_t filesize){
    uint8_t rdk[Nbyte];
    uint8_t dataptr[Nbyte];
    //pad the block

    //int thd = blockDim.x*blockIdx.x+threadIdx.x;
    int round=0;
    for(int pos=0;pos<filesize;pos+=Nbyte){
// handle the tail of the file	
	memset(dataptr,0x00,Nbyte);
	for(int i=0;i<Nbyte && pos+i < filesize; i++)
	    dataptr[i] =  buffer[pos+i];

	for(round=0;round<Nrd+1;round++){
	    //rdk=roundkey+round;
	    for(int i=0;i<Nrow;i++)
		for(int j=0;j<Ncol;j++)
		    rdk[i*Ncol+j]=roundkey[round][i][j];
	    if(round == 0)
		addRoundKey(dataptr,rdk);
	    else if(round == 10){
		byteSub(dataptr);
		shiftRow(dataptr);
		addRoundKey(dataptr,rdk);
	    }
	    else {
		byteSub(dataptr);
		shiftRow(dataptr);
		mixCol(dataptr);
		addRoundKey(dataptr,rdk);
		//printf("\tmixcol %d\n",round);
	    }
	}
	size_t size = 0;	
	if(filesize-pos < Nbyte)
	    size = filesize-pos;
	else
	    size = Nbyte;
	memcpy(buffer+pos, dataptr,size);
    }
    //printf("encrypting block %d,GPU\n",pos);
}





















