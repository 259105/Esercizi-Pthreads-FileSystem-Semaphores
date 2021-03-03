#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<ctype.h>
#include<semaphore.h>

typedef struct data_s{
	sem_t *sem_A,*sem_B,*sem_C,*sem_D,*sem_E,*sem_F,*sem_G,*sem_H,*sem_I;
}data_t;

void* A(void* data){
	sem_t *sA=((data_t *)data)->sem_A;
	sem_t *sB=((data_t *)data)->sem_B;
	sem_t *sC=((data_t *)data)->sem_C;
	sem_t *sD=((data_t *)data)->sem_D;
	sleep(1);
	while(1){
		sem_wait(sA);
		printf("A\n");
		sem_post(sB);
		sem_post(sC);
		sem_post(sD);
		sleep(5);
		printf("\n");
	}
}

void* B(void* data){
	sem_t *sB=((data_t *)data)->sem_B;
	sem_t *sI=((data_t *)data)->sem_I;
	while(1){
		sem_wait(sB);
		printf("B\n");
		sem_post(sI);
	}
}

void* C(void* data){
	sem_t *sC=((data_t *)data)->sem_C;
	sem_t *sE=((data_t *)data)->sem_E;
	sem_t *sF=((data_t *)data)->sem_F;
	while(1){
		sem_wait(sC);
		printf("C\n");
		sem_post(sE);
		sem_post(sF);
	}
}

void* D(void* data){
	sem_t *sD=((data_t *)data)->sem_D;
	sem_t *sH=((data_t *)data)->sem_H;
	while(1){
		sem_wait(sD);
		printf("D\n");
		sem_post(sH);
	}
}

void* E(void* data){
	sem_t *sE=((data_t *)data)->sem_E;
	sem_t *sG=((data_t *)data)->sem_G;
	while(1){
		sem_wait(sE);
		printf("E\n");
		sem_post(sG);
	}
}

void* F(void* data){
	sem_t *sF=((data_t *)data)->sem_F;
	sem_t *sG=((data_t *)data)->sem_G;
	while(1){
		sem_wait(sF);
		printf("F\n");
		sem_post(sG);
	}
}

void* G(void* data){
	sem_t *sI=((data_t *)data)->sem_I;
	sem_t *sG=((data_t *)data)->sem_G;
	while(1){
		sem_wait(sG);
		sem_wait(sG);
		printf("G\n");
		sem_post(sI);
	}
}

void* H(void* data){
	sem_t *sH=((data_t *)data)->sem_H;
	sem_t *sI=((data_t *)data)->sem_I;
	while(1){
		sem_wait(sH);
		printf("H\n");
		sem_post(sI);
	}
}

void* I(void* data){
	sem_t *sA=((data_t *)data)->sem_A;
	sem_t *sI=((data_t *)data)->sem_I;
	while(1){
		sem_wait(sI);
		sem_wait(sI);
		sem_wait(sI);
		printf("I\n");
		sem_post(sA);
	}
}

int main(int argc,char **argv){
	data_t data;
	pthread_t *tid=malloc(sizeof(pthread_t)*9);
	
	setbuf(stdout,0);
	data.sem_A=malloc(sizeof(sem_t));
	data.sem_B=malloc(sizeof(sem_t));
	data.sem_C=malloc(sizeof(sem_t));
	data.sem_D=malloc(sizeof(sem_t));
	data.sem_E=malloc(sizeof(sem_t));
	data.sem_F=malloc(sizeof(sem_t));
	data.sem_G=malloc(sizeof(sem_t));
	data.sem_H=malloc(sizeof(sem_t));
	data.sem_I=malloc(sizeof(sem_t));
	
	sem_init(data.sem_A,1,1);
	sem_init(data.sem_B,1,0);
	sem_init(data.sem_C,1,0);
	sem_init(data.sem_D,1,0);
	sem_init(data.sem_E,1,0);
	sem_init(data.sem_F,1,0);
	sem_init(data.sem_G,1,0);
	sem_init(data.sem_H,1,0);
	sem_init(data.sem_I,1,0);
	

	pthread_create(&(tid[0]),NULL,A,(void*)&data);
	pthread_create(&(tid[1]),NULL,B,(void*)&data);
	pthread_create(&(tid[2]),NULL,C,(void*)&data);
	pthread_create(&(tid[3]),NULL,D,(void*)&data);
	pthread_create(&(tid[4]),NULL,E,(void*)&data);
	pthread_create(&(tid[5]),NULL,F,(void*)&data);
	pthread_create(&(tid[6]),NULL,G,(void*)&data);
	pthread_create(&(tid[7]),NULL,H,(void*)&data);
	pthread_create(&(tid[8]),NULL,I,(void*)&data);
	
	for(int i=0;i<9;i++){
		pthread_join(tid[8],NULL);
	}
	
	return 0;
}
