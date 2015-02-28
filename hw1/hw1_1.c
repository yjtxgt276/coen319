#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#define PATTERN "GAAAAT"
#define NL '\n'

void* find_pattern(void* thread_number);
void file_size(FILE* fp, long* size);
FILE* fp_in;
long thd_c, fsz, match_c;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv[]){
    long thd_n;
    pthread_t* thd_hdls;
    struct timeval start_time, end_time;
    double exe_time=0.0;

    gettimeofday(&start_time,NULL);
    if((fp_in = fopen(argv[2],"r")) == NULL)
	    printf("Err: cannot open file %s\n", argv[2]);
    file_size(fp_in,&fsz);
    match_c=0;
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
    printf("The number of threads: %ld\n",thd_c);
    printf("The number of total matches is %ld\n",match_c);
    printf("Execution time is: %f secs\n",exe_time);
    return 0;
}

void* find_pattern(void* thread_number){
    char temp[sizeof(PATTERN)];
    char* p_nl;
    long curr = (long) thread_number*sizeof(char);
    long col_n = (long) thread_number*sizeof(char);
    long line_n = 0;

    pthread_mutex_lock(&mutex);
    printf("thread %ld is going\n",(long)thread_number);
    while(curr < fsz){
	fseek(fp_in,curr,SEEK_SET);	
	if(fgets(temp,sizeof(temp),fp_in)==NULL)
		printf("Err: cannot get strings\n");
	if ( ((p_nl=strrchr(temp, NL))!=NULL) && ( (p_nl-temp) <= thd_c) ){
	    line_n++;
	    col_n = (temp-p_nl-1);
	}
	if(strcmp(temp,PATTERN)==0){
	    printf("found a match at row %ld, column %ld\n", line_n, col_n);
	    match_c++;
	}
	col_n += thd_c*sizeof(char);
	curr += thd_c*sizeof(char);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void file_size(FILE* fp, long* size){
    fseek(fp,0L,SEEK_END);
    *size = (long)ftell(fp);
    return ;
}
