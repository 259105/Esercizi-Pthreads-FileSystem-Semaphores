#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SIZE_BUFF 1024

int cpFile(char *path1,char *path2){
	int fd,fs;
	int nr,nw; // numero di byte letti
	char buff[SIZE_BUFF];
	if((fs=open(path1,O_RDONLY))<0 || (fd=open(path2,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR,S_IWUSR))<0){
		fprintf(stderr,"Errore creazione file %s",path1);
		exit(-1);
	}
	while((nr=read(fs,&buff,SIZE_BUFF))>0){
		nw=write(fd,buff,nr);
		if(nr!=nw){
			fprintf(stderr,"Errore copia file %s",path1);
			exit(-1);
		}
	}
	if(nr<0){
		fprintf(stderr,"Errore copia file %s",path1);
		exit(-1);
	}
	return 0;
}

int cpDirectoryR(char *path1,char *path2,struct stat *buffSrc){
	DIR *dp;
	struct dirent *dirp;
	char fullpathSrc[258],fullpathDst[258];
	
	if((dp=opendir(path1))==NULL){
		fprintf(stderr,"Errore apertura dei direttori\n");
		exit(-1);
	}
	
	while((dirp=readdir(dp))!=NULL){
		if(strcmp(dirp->d_name,".")==0 || strcmp(dirp->d_name,"..")==0)
			continue;
		// Concateno il path con il nome del file interno
		sprintf(fullpathSrc,"%s/%s",path1,dirp->d_name);
		sprintf(fullpathDst,"%s/%s",path2,dirp->d_name);
		// Ricavo le statistiche dell' i-node
		if(lstat(fullpathSrc,buffSrc)<0){
			fprintf(stderr,"Errore stat %s\n",fullpathSrc);
			exit(-1);
		}
		if(S_ISDIR(buffSrc->st_mode)){
			// creo sotto-direttorio
			if(mkdir(fullpathDst,S_IRUSR|S_IWUSR|S_IXUSR)<0){
				fprintf(stderr,"Errore creazione sotto-direttorio %s\n",fullpathDst);
				exit(-1);
			}
			//ricorro con la copia del sotto-direttorio
			if(cpDirectoryR(fullpathSrc,fullpathDst,buffSrc)<0) exit(-1);
		}else if(S_ISREG(buffSrc->st_mode)){
			//copia file
			if(cpFile(fullpathSrc,fullpathDst)<0) exit(-1); 
		}
	}
	
	return 0;
}

int main(int argc, char *argv[]){
	if(argc!=3)
		exit(-1);
		
	struct stat buffDest,buffSrc;
	
	// Prendo stat dei path
	if(lstat(argv[1],&buffDest)==-1 || lstat(argv[2],&buffSrc)==-1){
		fprintf(stderr,"Errore stat dest/sourc)\n");
		exit(-1);
	}
	// Controllo che siano dei direttori
	if(!S_ISDIR(buffDest.st_mode)||!S_ISDIR(buffSrc.st_mode)){
		fprintf(stderr,"Errore i path inseriti non sono dei direttori\n");
		exit(-1);
	}
	
	//Apro ricorsivamente i direttori e copio con la funzione cpDirectoryR
	if(cpDirectoryR(argv[1],argv[2],&buffSrc)<0){
		fprintf(stderr,"Errore nella copia del direttorio\n");
		exit(-1);
	}
	
	return 0;
}
