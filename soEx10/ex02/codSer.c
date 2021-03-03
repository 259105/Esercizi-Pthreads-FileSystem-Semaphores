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
	
	//Apertura file ASCII
	FILE *fp=fopen(argv[1],"r");
	if(fp==NULL){
		fprintf(stderr,"Errore apertura file\n");
		exit(1);
	}
	//apertura file binario
	int fb=open(strcat(argv[1],".bin"),O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(fp<0){
		fprintf(stderr,"Errore apertura file\n");
		exit(1);
	}
	
	mon_t mon;
	int check;
	while(fscanf(fp,"%f %f %d",&mon.c,&mon.x,&mon.n)>0){
			//printf("%f %f %d\n",mon.c,mon.x,mon.n);
			check=write(fb,&mon,sizeof(mon_t));
			printf("Byte scritti: %d\n",check);
			if(check!=sizeof(mon_t))
				printf("Errore copia struct");
	}
	
	close(fb);
	fclose(fp);
	return 0;
}
