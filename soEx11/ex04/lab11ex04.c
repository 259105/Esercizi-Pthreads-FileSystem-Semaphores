#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

typedef struct{
	int *v;
	int len;
}thread_par;

void* thread(void* par);
void merge(int *v,int l,int m,int r);

int main(int argc,char **argv){
	setbuf(stdout,0);
	setbuf(stderr,0);
	int len=argc-1;
	int *v=malloc(sizeof(int)*len);
	for(int i=1;i<argc;i++)
		v[i-1]=atoi(argv[i]);
	if(!len) exit (1);
	if(len==1) exit(0);
	
	pthread_t tid;
	thread_par vet;
	vet.v=v; vet.len=len;
	pthread_create(&tid,NULL,thread,(void *)&(vet));
	pthread_join(tid,NULL);
	
	for(int i=0;i<len;i++)
		printf("%d ",v[i]);
	printf("\n");
		
	free(v);
	exit(0);
}

void* thread(void* par){
	thread_par *vect=(thread_par*) par;
	#ifdef debug
	printf("Ordino vettore:\n");
	for(int i=0;i<vect->len;i++)
		printf("%d ",vect->v[i]);
	printf("\n");
#endif
	if(vect->len==1)
		pthread_exit(NULL);
	int len=vect->len;
	pthread_t tid[2];
	thread_par vet[2];
	vet[0].v=vect->v; vet[0].len=len/2;
	vet[1].v=&(vect->v[len/2]); vet[1].len=len/2;
	pthread_create(&(tid[0]),NULL,thread,(void *)&(vet[0]));
	pthread_create(&(tid[1]),NULL,thread,(void *)&(vet[1]));
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
#ifdef debug
	printf("Faccio merge dei vettori:\n");
	for(int i=0;i<len;i++)
		printf("%d ",vect->v[i]);
	printf("\n");
#endif
	merge(vect->v,0,len/2,len);
#ifdef debug
	printf("Vettore ordinato: ");
	for(int i=0;i<len;i++)
		printf("%d ",vect->v[i]);
	printf("\n");
#endif
	pthread_exit(NULL);
}

void merge(int *v,int l,int m,int r){
	int *a=malloc(sizeof(int)*r);
	int k=0,i=0,j=m;
#ifdef debug
	printf("l=%d, m=%d, r=%d\n vettore={%d,%d}",l,m,r,v[0],v[1]);
#endif
	while(k<r){
		if(i==m) a[k++]=v[j++];
		else if(j==r) a[k++]=v[i++];
		else if(v[i]<v[j]) a[k++]=v[i++];
		else a[k++]=v[j++];
	}
	for(int i=0;i<r;i++)
		v[i]=a[i];

	free(a);
	return;
}

