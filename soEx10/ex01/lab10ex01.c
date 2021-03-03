#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<semaphore.h>
#include<ctype.h>

#define MAXBUFF 256

typedef struct data_thread_s{
	pthread_t tids[3];
	FILE *fpIn,*fpOut;
	char *buffIn[MAXBUFF]; //Buffer Circolare tra lettore e computatore
	int buffInWrite,buffInRead;
	char *buffOut[MAXBUFF];//Buffer circolare tra Computatore e Scrittore
	int buffOutWrite,buffOutRead;
}data_thread_t;

void initData(data_thread_t *d){
	d->buffInWrite=d->buffOutWrite=d->buffOutRead=d->buffInRead=0;
}

sem_t me1,s1Full,s1Empty,me2,s2Full,s2Empty,start;

void initSemaphores	(){
	sem_init(&me1,0,1); //mutex per il buffer condiviso, serve per la variabile nBuff1
	sem_init(&s1Full,0,0);
	sem_init(&s1Empty,0,MAXBUFF);
	
	sem_init(&me2,0,1); //mutex per il buffer condiviso, serve per la variabile nBuff1
	sem_init(&s2Full,0,0);
	sem_init(&s2Empty,0,MAXBUFF);
	sem_init(&start,0,0);
}

void freeSemaphores(){
	sem_destroy(&me1);
	sem_destroy(&me2);
	sem_destroy(&s1Full);
	sem_destroy(&s2Full);
	sem_destroy(&s1Empty);
	sem_destroy(&s2Empty);
	sem_destroy(&start);
}

int fine=0; // indica la fine del programma

void* reader(void* data);
void* computer(void* data);
void* writer(void* data); 


int main(int argc,char **argv){
	data_thread_t d_thread;
	d_thread.fpIn=fopen(argv[1],"r");
	d_thread.fpOut=fopen(argv[2],"w");
	initSemaphores();
	initData(&d_thread);
	setbuf(stdout,0);
	
	
	pthread_create(&(d_thread.tids[2]),NULL,writer,(void *)&d_thread);
	pthread_create(&(d_thread.tids[1]),NULL,computer,(void *)&d_thread);
	pthread_create(&(d_thread.tids[0]),NULL,reader,(void *)&d_thread);
	
	sleep(1);
	for(int i=3;i>0;i--){
		sem_post(&start);
	}
	
	pthread_join(d_thread.tids[0],NULL);
	pthread_join(d_thread.tids[1],NULL);
	pthread_join(d_thread.tids[2],NULL);
	
	freeSemaphores();
	
	fclose(d_thread.fpIn);
	fclose(d_thread.fpOut);
	return 0;
}

char* readLine(FILE *f);

void* reader(void *data){
	sem_wait(&start);
	data_thread_t *d=(data_thread_t*)data;
	int n;
	while(1){
		char *line=readLine(d->fpIn);
		if(line==NULL){
			sem_post(&s1Full);
			fine=1;
			pthread_exit(NULL);
		}
		sem_wait(&s2Empty);
		sem_wait(&me1);
			n=(d->buffInWrite++) % MAXBUFF;
			d->buffIn[n]=line;
			printf("Linea letta: %s\n",line);
		sem_post(&me1);
		sem_post(&s1Full);
		
		
	}
}

char* readLine(FILE *f){
	char c,line[100];
	int len=0;
	if(fscanf(f,"%c",&c)<0)
		return NULL;
	while(c!='\n'){
		line[len++]=c;
		if(fscanf(f,"%c",&c)<0)
			return NULL;
	}
	line[len]='\0';
	return strdup(line); // Potrebbe dare errori
}

char* swipeLine(char *line);

void* computer(void *data){
	sem_wait(&start);
	data_thread_t *d=(data_thread_t*)data;
	char *swapped;
	int n;
	while(1){
		sem_wait(&s1Full);
			if(fine && d->buffInWrite-d->buffInRead<=0){
				sem_post(&s2Full);
				pthread_exit(NULL);
			}
		sem_wait(&s2Empty);
		
		sem_wait(&me1);
			n=((d->buffInRead++) % MAXBUFF);
			swapped=d->buffIn[n];
		sem_post(&me1);
		
		swapped=swipeLine(swapped);
		printf("Line swipata: %s\n",swapped);
		sem_wait(&me2);
			n= (d->buffOutWrite++) % MAXBUFF;
			d->buffOut[n]=swapped;
		sem_post(&me2);
		
		sem_post(&s2Full);	
		sem_post(&s1Empty);
		
	}
}

char* swipeLine(char *line){
	int len=strlen(line);
	char* swappedLine=malloc(sizeof(char)*len);
	for(int i=len-1,j=0;i>=0;i--)
		swappedLine[j++]=toupper(line[i]);
	return swappedLine;
}

void* writer(void *data){
	sem_wait(&start);
	data_thread_t *d=(data_thread_t*)data;
	char* line;
	int n;
	while(1){
		sem_wait(&s2Full);
			if(fine && d->buffOutWrite-d->buffOutRead<=0) pthread_exit(NULL);
		sem_wait(&me2);
			n=(d->buffOutRead++) % MAXBUFF;
			line=d->buffOut[n];
			printf("Linea stampata: %s\n",line);
		sem_post(&me2);
		sem_post(&s2Empty);
		
		
		fprintf(d->fpOut,"%s\n",line);
		
	}
}

