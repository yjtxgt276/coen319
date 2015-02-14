#include "cpuaeslib.h"

void addRoundKey(uint8_t* block, uint8_t* roundKey ){
    for(int i=0;i<Nbyte;i++)
	block[i] ^= roundKey[i];
    //printf("hello from addRoundKey, GPU\n");
}

void byteSub(uint8_t* block){
//Nbyte=128/8=16, number of bytes in a block
    for(int i=0;i<Nrow;i++)
	for(int j=0;j<Ncol;j++){
	    block[i*Ncol+j]=sbox[block[i*Ncol+j]];
	    //printf("%x\n",block[i*Ncol+j]);
	}
 //   printf("Hello from GPU byteSub\n");
}

void invByteSub(uint8_t* block){
//Nbyte=128/8=16, number of bytes in a block
    for(int i=0;i<Nrow;i++)
	for(int j=0;j<Ncol;j++)
	    block[i*Ncol+j]=inv_sbox[block[i*Ncol+j]];
    //printf("Hello from GPU byteSub\n");
}

void shiftRow(uint8_t* block){
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

void invShiftRow(uint8_t* block){
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

void mixCol(uint8_t* block){
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

void invMixCol(uint8_t* block){
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

uint8_t gfmultiply(uint8_t a, uint8_t b){
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

void transform(uint8_t rki[Nrow][Ncol], uint8_t* temp, int round){
//round constant
    uint8_t rc=roundconst[round];
//shift the col up by 1 ele, and sub with sbox
    for(int i=0;i<Nrow;i++)
	temp[i]=sbox[ rki[((i+1)%Nrow)][Ncol-1] ];
    temp[0] = temp[0] ^ rc;
    return ;
}

void expandKey(void* k, uint8_t rk[Nrd+1][Nrow][Ncol])
{
    uint8_t* kp = (uint8_t*) k;
    uint8_t key[Nrow][Ncol]; 
//pad the key
    uint8_t temp[Nrow];
//input key -> matrixed key
    for(int i=0;i<Nrow;i++)
	for(int j=0;j<Ncol;j++){
	    key[i][j] = kp[i*Nrow+j]; 
	    //printf("%x\t",key[i][j]);
	}
//expand the key
    for(int round=0;round<Nrd+1;round++)
    {
//round 0
	if(round==0)
	    for(int row=0;row<Nrow;row++)
		for(int col=0;col<Ncol;col++)
		    rk[round][row][col]=key[row][col];    	
// other rounds
	else
	{
	    for(int col=0;col<Ncol;col++)
	    {
//first col of each roundkey
		if(col==0)
		{
		    transform(rk[round-1],temp,round);
		    for(int row=0;row<Nrow;row++)
			rk[round][row][col]=rk[round-1][row][col] ^ temp[row]; 
		}
//other cols of each roundkey
		else
		    for(int row=0;row<Nrow;row++)
			rk[round][row][col]=rk[round-1][row][col] ^ rk[round][row][col-1]; 
	    }	
	}
    }
}

off_t fsize(char* filename){
    struct stat st;
    if(stat(filename,&st)==0)
	return st.st_size;
    else 
	return -1;
}

void printx(void* data,size_t datasize){ 
    printf("on host:\n");
    for(int i=0;i<datasize;i++){
	printf("%x",*((uint8_t*)data+i));
    }
    printf("\n");
}
