#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

//Strutture utili:
typedef struct{
	pthread_t tid[3+1];
	int fp;
	float monomio[3];
}thread_data_t;

typedef struct mon_s{
	float c,x;
	int n;
}mon_t;

//Threads
void* primo(void *par);
void* secondo(void *par);
void* terzo(void *par);
void* quarto(void *par);

//Prototipi funzioni utili
float monomio(mon_t mon);
mon_t* readMonomio(int fp,mon_t *mon);
#ifdef DEBUG
void printMonomio(mon_t mon,int i);
#endif
float sommaMon(thread_data_t *d);

//Semaphores
sem_t s1,s2,s3,s4,meM1,meM2,meM3;

void semaphores_init(){
	// sono tutti mutex per avere un'accesso sequenziale all'file
	sem_init(&s1,0,1);
	sem_init(&s2,0,0);
	sem_init(&s3,0,0);
	sem_init(&s4,0,0);
	sem_init(&meM1,0,1);
	sem_init(&meM2,0,1);
	sem_init(&meM3,0,1);
}

void semaphores_destroy(){
	sem_destroy(&s1);
	sem_destroy(&s2);
	sem_destroy(&s3);
	sem_destroy(&s4);
	sem_destroy(&meM1);
	sem_destroy(&meM2);
	sem_destroy(&meM3);
}

//Variabili globali
int end=0;

int main(int argc,char **argv){
	setbuf(stdout,0);
	setbuf(stderr,0);
	if(argc!=2){
		fprintf(stderr,"Errore parametri inseriti");
		exit(1);
	}
	
	thread_data_t t_d;
	t_d.fp=open(argv[1],O_RDONLY,S_IRUSR|S_IWUSR);
	semaphores_init();
	
	#ifdef DEBUG
		printf("Avvio threads..\n");
	#endif
	
	pthread_create(&(t_d.tid[0]),NULL,primo,(void *)&t_d);
	pthread_create(&(t_d.tid[1]),NULL,secondo,(void *)&t_d);
	pthread_create(&(t_d.tid[2]),NULL,terzo,(void *)&t_d);
	pthread_create(&(t_d.tid[3]),NULL,quarto,(void *)&t_d);
	
	pthread_join(t_d.tid[0],NULL);
	pthread_join(t_d.tid[1],NULL);
	pthread_join(t_d.tid[2],NULL);
	pthread_join(t_d.tid[3],NULL);
	
	semaphores_destroy();
	close(t_d.fp);
	return 0;
}

float monomio(mon_t mon){
	float x=1;
	for(int i=0;i<mon.n;i++)
		x*=mon.x;
	return x*mon.c;
}

mon_t* readMonomio(int fp,mon_t *mon){
	int check=read(fp,mon,sizeof(mon_t));
	if(check<=0){
		//FILE finito o non ho letto niente e c'è un errore
		#ifdef DEBUG
			printf("Byte letti: %d\n",check);
		#endif
		mon=NULL;
		return mon;
	}else if(check!=sizeof(mon_t)){
		fprintf(stderr,"Errore lettura file binario\n");
	}
	
	return mon;
}

#ifdef DEBUG
void printMonomio(mon_t mon,int i){
	fprintf(stdout,"Monomio Letto da %d thread: %f %f %d\n",i,mon.c,mon.x,mon.n);
}
#endif

float sommaMon(thread_data_t *d){
	float sum=0;
	for(int i=0;i<3;i++){
		sum+=d->monomio[i];
		d->monomio[i]=0;
	}	
	return sum;
}

void* primo(void *par){
	thread_data_t *d=(thread_data_t *)par;
	mon_t *mon=malloc(sizeof(mon_t));
	while(1){
		#ifdef DEBUG
			printf("Avvio Primo thread..\n");
		#endif
		sem_wait(&s1);
			mon=readMonomio(d->fp,mon);
			#ifdef DEBUG
				if(mon!=NULL)
					printMonomio(*mon,1);
			#endif
			if(mon==NULL){
				/*Il file è finito e bisogna terminare in modo ordinato
				* uso la tecnica della variabile globale end
				*/
				end=1;
				sem_post(&s2);
				sem_post(&s3);
				for(int i=0;i<3*2;i++)
					sem_post(&s4);
				// Perchè metto il doppio??? Facendo così al quarto do la possbilità di fare un'altro giro e di accorgersi che non c'è più lavoro da fare..	
				//non funziona implementare con semaforo....
				
				pthread_exit(NULL);
			}
		sem_wait(&meM1);
			d->monomio[0]=monomio(*mon);
			#ifdef DEBUG
				printf("Risultato Primo Monomio: %f\n",d->monomio[0]);
			#endif
		sem_post(&s2);
		sem_post(&s4);
	}
}

void* secondo(void *par){
	thread_data_t *d=(thread_data_t *)par;
	mon_t *mon=malloc(sizeof(mon_t));
	while(1){
		#ifdef DEBUG
			printf("Avvio Secondo thread..\n");
		#endif
		sem_wait(&s2);
			if(end==1) pthread_exit(NULL);
			mon=readMonomio(d->fp,mon);
			#ifdef DEBUG
				if(mon!=NULL)
					printMonomio(*mon,2);
			#endif
		sem_wait(&meM2);
			if(mon!=NULL)
				d->monomio[1]=monomio(*mon);
			else
				d->monomio[1]=0;
			#ifdef DEBUG
				printf("Risultato Secondo Monomio: %f\n",d->monomio[1]);
			#endif
		sem_post(&s3);
		sem_post(&s4);
	}
}

void* terzo(void *par){
	thread_data_t *d=(thread_data_t *)par;
	mon_t *mon=malloc(sizeof(mon_t));
	while(1){
		#ifdef DEBUG
			printf("Avvio Terzo thread..\n");
		#endif
		sem_wait(&s3);
			if(end==1) pthread_exit(NULL);
			mon=readMonomio(d->fp,mon);
			#ifdef DEBUG
				if(mon!=NULL)
					printMonomio(*mon,3);
			#endif
		sem_wait(&meM3);
			if(mon!=NULL)
				d->monomio[2]=monomio(*mon);
			else
				d->monomio[2]=0;
			#ifdef DEBUG
				printf("Risultato Terzo Monomio: %f\n",d->monomio[2]);
			#endif
		sem_post(&s1);
		sem_post(&s4);
	}
}

void* quarto(void *par){
	thread_data_t *d=(thread_data_t *)par;
	float sum;
	while(1){
		for(int i=0;i<3;i++)
				sem_wait(&s4);
		#ifdef DEBUG
			printf("Avvio Quarto thread..\n");
		#endif
		sum=sommaMon(d);
		if(!sum) pthread_exit(NULL);
		sem_post(&meM3);
		sem_post(&meM2);
		sem_post(&meM1);
		printf("%f\n",sum);
		
	}
}
