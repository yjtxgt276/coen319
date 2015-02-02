#include "keySchedule.h"

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
