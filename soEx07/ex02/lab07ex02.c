#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFF 128
#define BUFF_READ 4096

typedef struct{
	char *nameIn;
	char *nameOut;
	int tid;
	int ret;
} options_t;

void* makeEsortFile(void *par){
	//Faccio casting
	options_t *opt=(options_t *) par;
	//Setto variabili
	int tid=opt->tid;
	//Faccio open e controllo
	FILE *fin=fopen(opt->nameIn,"r");
	FILE *fout=fopen(opt->nameOut,"w+");
	if(fin==NULL || fout==NULL){
		fprintf(stderr,"Errore apertura file output\n");
		pthread_exit( (void*) &tid);
	}
	
	int n; // numero di numeri da ordinare
	//Leggo primo intero che indica numero di numeri
	fscanf(fin,"%d",&n);
	fprintf(fout,"%d\n",n);
	
	int *v=malloc(sizeof(int)*n);
	
	for(int i=0;i<n;i++){
		if(fscanf(fin,"%d",&v[i])<=0){
			fscanf(stderr,"Errore lettura numeri\n");
			pthread_exit((void*) &tid);
		}
	}
	
	
	//Ordino file Con Insertion Sort
	for(int i=1;i<n;i++){
		int x=v[i];		
		int j=i-1;
		while(j>=0 && v[j]>x){
			v[j+1]=v[j];
			j--;
		}
		v[j+1]=x;
	}
	
	for(int i=0;i<n;i++)
		fprintf(fout,"%d\n",v[i]);
	fclose(fin);
	fclose(fout);
	opt->ret=-1;
	pthread_exit((void*) &(opt->ret));
}

int main(int argc, char **argv){
	setbuf(stdout,0);
	if(argc!=4){
		fprintf(stderr,"Error: parametri inseriti non validi..\nInserire n° file, fileIn, fileOut\n");
		exit(EXIT_FAILURE);	
	}
	
	int n=atoi(argv[1]);
	pthread_t *tids=malloc(sizeof(pthread_t)*n);
	options_t *o=malloc(sizeof(options_t)*n);
	char nameIn[128],nameOut[128];
	
	for(int i=1;i<=n;i++){
		sprintf(nameIn,"%s%d.txt",argv[2],i);
		sprintf(nameOut,"%s%d.txt",argv[3],i);
		o[i-1].nameIn=strdup(nameIn);
		o[i-1].nameOut=strdup(nameOut);
		o[i-1].tid=i-1;
		
		//printf("Creo %d thread\n",i);
		
		pthread_create(&(tids[i-1]),NULL,makeEsortFile,(void *)&(o[i-1]));
	}
	
	int *status;
	for(int i=0;i<n;i++){
		pthread_join(tids[i],(void **)&status);
		
		//printf("Join con %d thread\n",i);
		//printf("fanculo");
		//printf("%d\n",*(status));
		if( *status >=0){
			fprintf(stderr,"Errore JOIN Generico Copia e Ordinamento File: %s\n",o[i].nameIn);
			exit(1);
		}
		
	}
	
	for(int i=0;i<n;i++){
		free(o[i].nameIn);
		free(o[i].nameOut);
	}
	free(o);
	free(tids);
	return 0;
}
