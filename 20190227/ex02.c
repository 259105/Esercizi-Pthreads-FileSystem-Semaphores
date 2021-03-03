#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

typedef struct{
	int p[2];
}sem_t;

void sem_init(sem_t *s,int n){
	if(pipe(s->p)==-1){
		fprintf(stderr,"Errore creazione pipe\n");
		exit(1);
	}
	char c='X';
	for(int i=0;i<n;i++)
		if(write(s->p[1],&c,sizeof(char))==0){
			fprintf(stderr,"Errore scrittura in pipe\n");
			exit(1);
		}
	return;
}

void sem_wait(sem_t *s){
	char c='X';
	if(read(s->p[0],&c,sizeof(char))==0){
		fprintf(stderr,"Errore lettura pipe\n");
		exit(1);
	}
	return;
}

void sem_post(sem_t *s){
	char c='X';
	if(write(s->p[1],&c,sizeof(char))==0){
		fprintf(stderr,"Errore scrittura pipe\n");
		exit(1);
	}
	return;
}

void sem_destroy(sem_t *s){
	close(s->p[0]);
	close(s->p[1]);
}

void* thread(void *par);
void P1(FILE *fp,sem_t *s1,sem_t *s2);
void P2(FILE *fp,sem_t *s1,sem_t *s2);

int main(int argc,char **argv){
	if(argc!=2){
		fprintf(stderr,"Errore parametri inseriti\n");
		exit(1);
	}
	FILE *fp;
	if((fp=fopen(argv[1],"w+"))==NULL){
		fprintf(stderr,"Errore apertura file\n");
		exit(1);
	}
	setbuf(fp,0);
	sem_t s1,s2;
	sem_init(&s1,1); sem_init(&s2,0);
	if(fork()){
		P1(fp,&s1,&s2);
	}else{
		P2(fp,&s1,&s2);
	}
	wait(NULL);
	return(0);
}

void P1(FILE *fp,sem_t *s1,sem_t *s2){
	char str[100];
	while(1){
		rewind(fp);
		sem_wait(s1);
		
		printf("Lettura da P1\n");
		
		while(fscanf(fp,"%s",str)>0){
			if(!strcmp(str,"END"))
				return;
			fprintf(stdout,"%s\n",str);
		}
		rewind(fp);
		
		printf("Inserisci parola:\n");
		
		fscanf(stdin,"%s",str);
		
		while(strcmp(str,"end")){
			fprintf(fp,"%s\n",str);
			if(!strcmp(str,"END")){
				sem_post(s2);
				return;
			}
			
			printf("Inserisci parola:\n");
			
			fscanf(stdin,"%s",str);
		}
		sem_post(s2);
	}
}

void P2(FILE *fp,sem_t *s1,sem_t *s2){
	char str[100];
	while(1){
		rewind(fp);
		sem_wait(s2);
		
		printf("Lettura da P2\n");
		
		while(fscanf(fp,"%s",str)>0){
			if(!strcmp(str,"END"))
				exit(1);
			fprintf(stdout,"%s\n",str);
		}
		rewind(fp);
		
		printf("Inserisci parola:\n");
		
		fscanf(stdin,"%s",str);
		while(strcmp(str,"end")){
			fprintf(fp,"%s\n",str);
			if(!strcmp(str,"END")){
				sem_post(s1);
				exit(1);
			}
			
			printf("Inserisci parola:\n");
			
			fscanf(stdin,"%s",str);
		}
		sem_post(s1);
	}
}
