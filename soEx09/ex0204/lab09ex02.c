#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

typedef struct sem_s{
	int f[2];
}sem_t;

void sem_init(sem_t* s, int n, int value){
	pipe(s->f);
	char x='x';
	for(int i=0;i<value;i++){
		write(s->f[1],&x,sizeof(char));
	}
}

void sem_wait(sem_t* s){
	char x;
	read(s->f[0],&x,sizeof(char));
}

void sem_post(sem_t* s){
	char x='x';
	write(s->f[1],&x,sizeof(char));
}

void sem_destroy(sem_t *s){
	close(s->f[0]);
	close(s->f[1]);
}

typedef struct semaphore_s{
		sem_t *s1,*s2,*s3,*s4,*s5,*s6;
}semaphore_t;

void* A(void* sem){
	sem_t *s1=((semaphore_t *) sem)->s1;
	sem_t *s2=((semaphore_t *) sem)->s2;
	
	sem_wait(s1);
	printf("A\n");
	sem_post(s2);
	sem_post(s2);
	sem_post(s2);
	
	pthread_exit(NULL);
}

void* B(void* sem){
	sem_t *s6=((semaphore_t *) sem)->s6;
	sem_t *s2=((semaphore_t *) sem)->s2;
	
	sem_wait(s2);
	printf("B\n");
	sem_post(s6);
	pthread_exit(NULL);
}

void* C(void* sem){
	sem_t *s3=((semaphore_t *) sem)->s3;
	sem_t *s2=((semaphore_t *) sem)->s2;
	
	sem_wait(s2);
	printf("C\n");
	sem_post(s3);
	sem_post(s3);
	pthread_exit(NULL);
}

void* D(void* sem){
	sem_t *s4=((semaphore_t *) sem)->s4;
	sem_t *s2=((semaphore_t *) sem)->s2;
	
	sem_wait(s2);
	printf("D\n");
	sem_post(s4);
	pthread_exit(NULL);
}

void* E(void* sem){
	sem_t *s3=((semaphore_t *) sem)->s3;
	sem_t *s5=((semaphore_t *) sem)->s5;
	
	sem_wait(s3);
	printf("E\n");
	sem_post(s5);
	pthread_exit(NULL);
}

void* F(void* sem){
	sem_t *s3=((semaphore_t *) sem)->s3;
	sem_t *s5=((semaphore_t *) sem)->s5;
	
	sem_wait(s3);
	printf("F\n");
	sem_post(s5);
	pthread_exit(NULL);
}

void* G(void* sem){
	sem_t *s5=((semaphore_t *) sem)->s5;
	sem_t *s6=((semaphore_t *) sem)->s6;
	
	sem_wait(s5);
	sem_wait(s5);
	printf("G\n");
	sem_post(s6);
	pthread_exit(NULL);
}

void* H(void* sem){
	sem_t *s4=((semaphore_t *) sem)->s4;
	sem_t *s6=((semaphore_t *) sem)->s6;
	
	sem_wait(s4);
	printf("H\n");
	sem_post(s6);
	pthread_exit(NULL);
}

void* I(void* sem){
	sem_t *s1=((semaphore_t *) sem)->s1;
	sem_t *s6=((semaphore_t *) sem)->s6;
	
	sem_wait(s6);
	sem_wait(s6);
	sem_wait(s6);
	printf("I\n");
	sem_post(s1);
	pthread_exit(NULL);
}

int main(int argc,char ** argv){
	setbuf(stdout,0);
	pthread_t *tid=malloc(sizeof(pthread_t)*9);
	semaphore_t *semap=malloc(sizeof(semaphore_t));
	semap->s1=malloc(sizeof(sem_t));
	sem_init(semap->s1,1,1);
	semap->s2=malloc(sizeof(sem_t));
	sem_init(semap->s2,1,0);
	semap->s3=malloc(sizeof(sem_t));
	sem_init(semap->s3,1,0);
	semap->s4=malloc(sizeof(sem_t));
	sem_init(semap->s4,1,0);
	semap->s5=malloc(sizeof(sem_t));
	sem_init(semap->s5,1,0);
	semap->s6=malloc(sizeof(sem_t));
	sem_init(semap->s6,1,0);
	while(1){
		pthread_create(&(tid[0]),NULL,A,(void*)semap);
		pthread_create(&(tid[1]),NULL,B,(void*)semap);
		pthread_create(&(tid[2]),NULL,C,(void*)semap);
		pthread_create(&(tid[3]),NULL,D,(void*)semap);
		pthread_create(&(tid[4]),NULL,E,(void*)semap);
		pthread_create(&(tid[5]),NULL,F,(void*)semap);
		pthread_create(&(tid[6]),NULL,G,(void*)semap);
		pthread_create(&(tid[7]),NULL,H,(void*)semap);
		pthread_create(&(tid[8]),NULL,I,(void*)semap);
		pthread_join(tid[0],NULL);
		pthread_join(tid[1],NULL);
		pthread_join(tid[2],NULL);
		pthread_join(tid[3],NULL);
		pthread_join(tid[4],NULL);
		pthread_join(tid[5],NULL);
		pthread_join(tid[6],NULL);
		pthread_join(tid[7],NULL);
		pthread_join(tid[8],NULL);
		printf("\n");
		sleep(5);
	}
	
	sem_destroy(semap->s1);
	sem_destroy(semap->s2);
	sem_destroy(semap->s3);
	sem_destroy(semap->s4);
	sem_destroy(semap->s5);
	sem_destroy(semap->s6);
	
	return 0;
}
