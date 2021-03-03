#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* ./makeBinaryFile <nomeFile> <.> numeri da inserire <.>  */

int main(int argc,char*argv[]){
	if(argc==1){
		fprintf(stderr,"Errore paramtri inseriti non validi\n");
		exit(1);
	}
	int fd;
	if((fd=open(argv[1],O_RDONLY))<0){
		fprintf(stderr,"Errore lettura del file\n");
		exit(1);
	}
	int n;
	int rd=read(fd,&n,sizeof(int));
	fprintf(stdout,"Il file contiene %d numeri:\n",n);
	for(int i=0;i<n;i++){
		int k;
		rd=read(fd,&k,sizeof(int));
		if(rd!=sizeof(int)){
			fprintf(stderr,"Errore lettura del file");
			exit(1);
		}
		fprintf(stdout,"%d\n",k);
	}
	return 0;
}
