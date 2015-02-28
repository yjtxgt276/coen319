#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#define FILENAME "alphabet.txt"
#define PATTERN "GAAAAT"

void* find_pattern(void* thread_number);
void file_size(FILE* fp, long* size);
void file_preprocess(FILE* fp, struct** pp_letter);

FILE* fp_in;
long thd_c, fsz, line_c, match_c, el_c;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
struct letter{
    char c;
    long row_n;
    long col_n;
};

int main(int argc, char* argv[]){
    long thd_n;
    pthread_t* thd_hdls;
    struct timeval start_time, end_time;
    double exe_time=0.0;

    gettimeofday(&start_time,NULL);
    if((fp_in = fopen(argv[2],"r")) == NULL)
	    printf("Err: cannot open file %s\n", argv[2]);
    file_size(fp_in,&fsz);
    line_c=0;
    match_c=0;
    el_c=0;
    thd_c=strtol(argv[1],NULL,10);
    thd_hdls = malloc(thd_c*sizeof(pthread_t));

    for(thd_n=0;thd_n<thd_c;thd_n++)
	    pthread_create(&thd_hdls[thd_n],NULL,find_pattern, (void*)thd_n);

    for(thd_n=0;thd_n<thd_c;thd_n++)
	    pthread_join(thd_hdls[thd_n],NULL);
    free(thd_hdls);
    fclose(fp_in);
    gettimeofday(&end_time,NULL);
    exe_time = (end_time.tv_sec*1000000+end_time.tv_usec-start_time.tv_sec*1000000-start_time.tv_usec)/1000000.0;
    printf("line_c is %ld\n", line_c);
    printf("The number of total matches is %ld\n",match_c);
    printf("Execution time is: %f secs\n",exe_time);
    return 0;
}

void* find_pattern(void* thread_number){
    char temp[sizeof(PATTERN)];
    long curr = (long) thread_number*sizeof(char);
    long col_n = (long) thread_number*sizeof(char);
    long line_n = line_c;

    pthread_mutex_lock(&mutex);
    while(curr < fsz){

	fseek(fp_in,curr,SEEK_SET);	
	if(fgets(temp,sizeof(temp),fp_in)==NULL)
		printf("Err: cannot get strings\n");
	//printf("%s\n",temp);
	el_c++;
	if (temp[0] == '\n'){
    	    line_c++;
	//    printf("line %ld says hello\n",line_n);
	    col_n = -1;
	}
	if(strcmp(temp,PATTERN)==0){
	    printf("find a match at line %ld, column %ld\n", line_n, col_n);
	    match_c++;
	}
	col_n += thd_c*sizeof(char);
	curr += thd_c*sizeof(char);
    }
    pthread_mutex_unlock(&mutex);
    //printf("\nhello from thread%ld of %ld\n", num, thd_c);
    return NULL;
}

void file_size(FILE* fp, long* size){
    fseek(fp,0L,SEEK_END);
    *size = (long)ftell(fp);
    return ;
}

void file_preprocess(FILE* fp_in, struct** pp_letter){
    pp_letter = malloc(file_size(fp_in)*sizeof(letter));    
    while( ! feof(fp_in)){
	pp_letter->
    }
}
