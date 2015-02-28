#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define TARGET "alphabet.txt"

void* file_partition(FILE* fp_in, long thd_c, long* p_pos);
//void* file_output(FILE* fp_in,FILE* fp_out, long thd_n);
void* file_output(FILE* fp_in,FILE* fp_out, long* pos);
void* file_search(void* pos);
long count;

int main(int argc, char* argv[]){
    FILE* fp_in;
    long* pos;
printf("check point 1\n");
    count = strtol(argv[1],NULL,10);
    
printf("check point 2 : count %ld\n",count);
    fp_in = fopen(TARGET,"r");
printf("check point 3\n");
    pos = malloc(count*sizeof(long));
    file_partition(fp_in,count,pos);
printf("check point 4\n");
    file_output(fp_in,stdout,pos);
printf("check point 5\n");

    return 0;
}

void* file_partition(FILE* fp_in, long thd_c, long* p_pos){
    long size=0, i , pos[thd_c]; 

    fseek(fp_in,0L,SEEK_END);
    size = ftell(fp_in); 
    
    for(i=0; i < thd_c;i++){
	p_pos[i]=i*size/count;
    }

    return p_pos;
}
void* file_output(FILE* fp_in,FILE* fp_out, long* pos){
    char c;
    long i=0;

    while(pos != NULL && i < 10){
	fseek(fp_in,0L, *pos);
	printf("from byte %ld :\n",*pos);
	while(( c = fgetc(fp_in))!=EOF)
		fputc(c,fp_out);
	pos++;
	i++;
    }

    return;
}
void* file_search(void* pos){
    
}
