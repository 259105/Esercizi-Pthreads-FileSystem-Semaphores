#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

typedef struct mon_s{
	float c,x;
	int n;
}mon_t;

int main(int argc,char** argv){
	if(argc!=2){
		fprintf(stderr,"Errore parametri inseriti\n");
		exit(1);
	}
	
	//apertura file binario
	printf("apro file binario: %s\n",argv[1]);
	int fb=open(argv[1],O_RDONLY,S_IRUSR|S_IWUSR);
	if(fb==-1){
		fprintf(stderr,"Errore apertura file\n");
		exit(1);
	}
	
	//Apertura file ASCII
	FILE *fp=fopen(strcat(argv[1],".txt"),"w");
	if(fp==NULL){
		fprintf(stderr,"Errore apertura file\n");
		exit(1);
	}
	
	
	mon_t mon;
	int check;
	
	while((check=read(fb,&mon,sizeof(mon_t)))>0){
		printf("Byte letti: %d\n",check);
		printf("%f %f %d\n",mon.c,mon.x,mon.n);
		fprintf(fp,"%f %f %d\n",mon.c,mon.x,mon.n);
		if(check!=sizeof(mon_t))
			fprintf(stderr,"Errore copia");
		
	}
	printf("Byte letti: %d\n",check);
	close(fb);
	fclose(fp);
	return 0;
}
