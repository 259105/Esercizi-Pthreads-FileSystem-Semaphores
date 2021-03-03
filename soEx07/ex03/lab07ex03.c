#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<pthread.h>

typedef struct{
	int tid;
	int r;
	char* nameIn;
	int *v;
}Options_t;

void* makeEsortFile(void* par){
	Options_t *opt=(Options_t *)par;
	FILE *fin=fopen(opt->nameIn,"r");
	if(fin==NULL){
		fprintf(stderr,"Errore apertura file input: %s\n",opt->nameIn);
		opt->r=-1;
		pthread_exit( (void*) &(opt->r));
	}
	int n;
	fscanf(fin,"%d",&n);
	if(!n){
		fclose(fin);
		opt->r=n;
		pthread_exit((void*) &(opt->r));
	}
	opt->v=malloc(sizeof(int)*n);
	if(opt->v==NULL){
		fprintf(stderr,"Allocation Error of Vector\n");
		fclose(fin);
		opt->r=-2;
		pthread_exit((void*) &(opt->r));
	}
	for(int i=0;i<n;i++)
		fscanf(fin,"%d",&opt->v[i]);
	//Ordino con Insertion sort
	int j,x;
	for(int i=1;i<n;i++){
		x=opt->v[i];
		j=i-1;
		while(j>=0 && opt->v[j]>x){
			opt->v[j+1]=opt->v[j];
			j--;
		}
		opt->v[j+1]=x;
	}
	// return che è tutto ok
	fclose(fin);
	opt->r=n;
	pthread_exit((void*) &(opt->r));
}


int main(int argc,char **argv){

	setbuf(stdout,0);
	if(argc!=4){
		fprintf(stderr,"Error: parametri inseriti non validi..\nInserire n° file, fileIn, fileOut\n");
		exit(EXIT_FAILURE);	
	}
	
	int n=atoi(argv[1]);
	Options_t *opt=malloc(sizeof(Options_t)*n);
	pthread_t *tids=malloc(sizeof(pthread_t)*n);
	char file[1024];
	
	for(int i=0;i<n;i++){
		sprintf(file,"%s%d.txt",argv[2],i+1);
		opt[i].nameIn=strdup(file);
		opt[i].tid=i;
		pthread_create(&(tids[i]),NULL,makeEsortFile,(void*)&opt[i]);
	}
	
	int *r;
	int *v;
	int len=0;
	
	for(int i=0;i<n;i++){
		pthread_join(tids[i],(void **) &r);
		
		if(*r<0){
			fprintf(stderr,"Errore %d sul tread %d\n",*r,i);
			continue;
		}else if(*r==0)
			continue;
			
		// Primo elemento
		if(!i){
			len=*r;
			v=malloc(sizeof(int)*len);
			for(int j=0;j<len;j++)
				v[j]=opt[i].v[j];
			continue;
		}
			
		//Copio vettore
		int *t=malloc(sizeof(int)*len);
		for(int j=0;j<len;j++)
			t[j]=v[j];
		
		//Allargo il precedente
		int nr=len;
		len+=*r;
		v=realloc(v,sizeof(int)*len);
		
		printf("Merge tra vettore precedente:\n");
		for(int j=0;j<nr;j++)
			printf("%d ",t[j]);
		printf("\ne quello nuovo:\n");
		for(int j=0;j<*r;j++)
			printf("%d ",opt[i].v[j]);
		printf("\nRisultato:\n");
		
		//Faccio merge
		for(int m=0,n=0,k=0;m<nr || n<*r;)
			if(m<nr && t[m]<=opt[i].v[n])
				v[k++]=t[m++];
			else
				v[k++]=opt[i].v[n++];
			
		for(int j=0;j<len;j++)
			printf("%d ",v[j]);
		printf("\n");
		
		free(t);
	}
	
	//Stampo su file
	sprintf(file,"%s.txt",argv[3]);
	FILE *fout=fopen(file,"w+");
	if(fout==NULL){
		fprintf(stderr,"Errore apertura file output: %s\n",file);
		exit(1);
	}
	fprintf(fout,"%d\n",len);
	for(int i=0;i<len;i++){
		fprintf(fout,"%d\n",v[i]);
	}
	fclose(fout);
	
	free(v);
	for(int i=0;i<n;i++){
		free(opt[i].nameIn);
		free(opt[i].v);
	}
	free(opt);
	free(tids);
	return 0;
}
