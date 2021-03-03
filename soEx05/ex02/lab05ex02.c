#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>


int letturaEswipe(int fd,int j){
	int rd,wr,nDx,nSx;
  		//Sistemo puntatore file
  		lseek(fd,(j+1)*sizeof(int),SEEK_SET);
  		//Leggo numeri successivi
  		rd=read(fd,&nSx,sizeof(int));
  		wr=read(fd,&nDx,sizeof(int));
  		//Controllo che siano stati letti correttamente
  		if(rd!=sizeof(int) || wr!=sizeof(int)){
  			fprintf(stderr,"Errore letture file\n");
  			return 1;
  		}
//    	if (v[j] > v[j+1]) {
//       	val = v[j];
//       	v[j] = v[j+1];
//       	v[j+1] = val;
//    	}
    	if(nSx > nDx){
    		//scambio i 2 valori
    		//Sistemo puntatore file
  			lseek(fd,(j+1)*sizeof(int),SEEK_SET);
  			//scrivo sul file in ordine inverso
  			rd=write(fd,&nDx,sizeof(int));
  			wr=write(fd,&nSx,sizeof(int));
  			//Controllo che siano stati scritti correttamente
  			if(rd!=sizeof(int) || wr!=sizeof(int)){
  				fprintf(stderr,"Errore letture file\n");
  				return 1;
  			}
    	}
    	return 0;
}

int main(int argc,char *argv[]){
	if(argc!=2){
		fprintf(stderr,"Errore parametri inseriti non validi\n");
		exit(1);
	}
	int fd;
	if((fd=open(argv[1],O_RDWR))<0){
		fprintf(stderr,"Errore apertura file\n");
		exit(1);
	}
	int n; // numero di numeri
	int rd;
	//int rd,wr;// servono per fare controlli su byte letti e scritti
	//int nDx,nSx; // serve per mantenere temporanemanete il numero letto
	
	// LETTURA DIMENZIONE FILE
	rd=read(fd,&n,sizeof(int));
	if(rd!=sizeof(int)){
		fprintf(stderr,"Errore lettura file\n");
		exit(1);
	}
	
	// NON CONCORRENTE
	/*for (int i=0; i<n-1; i++) {
  	for (int j=0; j<n-1-i; j++) {
  		//Sistemo puntatore file
  		lseek(fd,(j+1)*sizeof(int),SEEK_SET);
  		//Leggo numeri successivi
  		rd=read(fd,&nSx,sizeof(int));
  		wr=read(fd,&nDx,sizeof(int));
  		//Controllo che siano stati letti correttamente
  		if(rd!=sizeof(int) || wr!=sizeof(int)){
  			fprintf(stderr,"Errore letture file");
  			exit(1);
  		}
//    	if (v[j] > v[j+1]) {
//       	val = v[j];
//       	v[j] = v[j+1];
//       	v[j+1] = val;
//    	}
    	if(nSx > nDx){
    		//scambio i 2 valori
    		//Sistemo puntatore file
  			lseek(fd,(j+1)*sizeof(int),SEEK_SET);
  			//scrivo sul file in ordine inverso
  			rd=write(fd,&nDx,sizeof(int));
  			wr=write(fd,&nSx,sizeof(int));
  			//Controllo che siano stati scritti correttamente
  			if(rd!=sizeof(int) || wr!=sizeof(int)){
  				fprintf(stderr,"Errore letture file");
  				exit(1);
  			}
    	}
  	}
	}*/
	
	pid_t pid=0;
	for (int i=0; i<n-1; i++) {
  	for (int j=0; j<n-1-i; j++) {
  		if(!(pid=fork())){
  			//IL FIGLIO
  			if(letturaEswipe(fd,j)==1){
  				fprintf(stderr,"Errore swipe\n");
  				exit(1);
  			}
  			exit(0);
  		}
  		waitpid(pid,NULL,0);
  	}
	}
	
	fprintf(stdout,"Stampa dei numeri in ordine:\n");
	execlp("./readBinaryFile","print",argv[1],NULL);
}
