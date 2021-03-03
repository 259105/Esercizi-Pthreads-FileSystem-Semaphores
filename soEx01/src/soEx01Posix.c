#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

#define BUFFSIZE 4096

int main(int argc,char *argv[]){

	char buff[BUFFSIZE];//numero di bite memorizzati
	int fs,fd;
	int nR,nW;
	
	fs=open(argv[1],O_RDONLY);
	fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	
	if(fs==-1){
		fprintf(stderr,"Errore file sorgente\n");
		exit(1);
	}
	if(fd==-1){
		fprintf(stderr,"Errore file destinazione\n");
		exit(1);
	}
	while((nR=read(fs,buff,BUFFSIZE))>0){
		nW=write(fd,buff,nR);
		if(nR!=nW)
			fprintf(stderr,"Errore: Letti %d, Scritti %d\n",nR,nW);
		
	}
	if(nR<0)
		fprintf(stderr,"Errore lettura\n");
	close(fs);
	close(fd);
	return(0);
}
