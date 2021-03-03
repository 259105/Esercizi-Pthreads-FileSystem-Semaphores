#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* ./makeBinaryFile <nomeFile> <.> numeri da inserire <.>  */

int main(int argc,char*argv[]){
	if(argc==1){
		fprintf(stderr,"Errore paramtri inseriti non validi\nInserire di seguito il nome del file e i numeri da inserire nel file binario\n");
		exit(1);
	}
	int fd;
	if((fd=open(argv[1],O_WRONLY|O_TRUNC|O_CREAT,S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP))<0){
		fprintf(stderr,"Errore creazione del file\n");
		exit(1);
	}
	int wr;
	int n=argc-2;
	//Scrivo quanto numeri sono presenti nel file
	wr=write(fd,&n,sizeof(int));
	if(wr!=sizeof(int)){
			fprintf(stderr,"Errore scrittura file\n");
			exit(1);
		}
	//Scrivo i numeri inseriti da riga di comando
	for(int i=2;i<argc;i++){
		n=atoi(argv[i]);
		wr=write(fd,&n,sizeof(int));
		if(wr!=sizeof(int)){
			fprintf(stderr,"Errore scrittura file\n");
			exit(1);
		}
	}
	return 0;
}
