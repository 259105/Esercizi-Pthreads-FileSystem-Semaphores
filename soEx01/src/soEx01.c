#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv[]){

	char c;
	FILE *fs,*fd;
	
	fs=fopen(argv[1],"r");
	fd=fopen(argv[2],"w");
	if(fs==NULL){
		fprintf(stderr,"Errore file sorgente errato o mancante\n");
		exit(1);
	}
	while(fscanf(fs,"%c",&c)>0){
		fprintf(fd,"%c",c);
	}
	fclose(fs);
	fclose(fd);
	return(0);
}
