#include "keySchedule.h"

int main(int argc, char** argv){
    char key[16];    
    memset(key,0x00,16);
    uint8_t roundkey[Nrd+1][Nrow][Ncol];
    /*
    roundkey = (uint8_t***) malloc(Nrd*sizeof(uint8_t**));
    for(int i=0;i<Nrd+1;i++){
	roundkey[i]=(uint8_t**) malloc(Nrow*sizeof(uint8_t*)); 
	for(int j=0;j<Nrow;j++){
	    roundkey[i][j]=(uint8_t*) malloc(Ncol*sizeof(uint8_t));
	}
    }

    for(int i=0;i<16;i++){
	key[i]=argv[1][i];
	//key[i]=0x00;
	printf("%x",key[i]);
    }
    */
    strncpy(key,argv[1],strlen(argv[1]));
    printf("key length: %d\n",strlen(argv[1]));
    printf("\n");
    expandKey(key,roundkey);

    for(int i=0;i<Nrd+1;i++){
	printf("round %d\n",i);
	for(int j=0;j<Nrow;j++){
	    for(int k=0;k<Ncol;k++){
		printf("%x\t",roundkey[i][j][k]);
	    }	
	    printf("\n");
	}
    }
    return 0;
}


