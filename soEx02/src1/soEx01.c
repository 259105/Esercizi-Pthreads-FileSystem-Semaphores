#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

int printDirectoryR(char *path,struct stat *buf,char *tab){
	DIR *dp;
	struct dirent *dirp;
	char fullname[257]; // come faccio a farlo dinamico??
	
	if((dp=opendir(path))==NULL){
		fprintf(stderr,"Errore apertura direttorio %s sbagliato\n",path);
		exit(-1);
	}
	
	while((dirp=readdir(dp))!=NULL){
		if(strcmp(dirp->d_name,".")==0 || strcmp(dirp->d_name,"..")==0)
			continue;
		// faccio concatenazione
		sprintf(fullname,"%s/%s",path,dirp->d_name);
		// printf("%s\n",fullname); //stampo
		if(lstat(fullname,buf)<0){
			fprintf(stderr,"Errore lettura %s\n",fullname);
			exit(-1);
		}
		
		if(S_ISREG(buf->st_mode)){
			fprintf(stdout,"%s %s\n",tab,dirp->d_name);
		}else{
			fprintf(stdout,"%s %s\n",tab,dirp->d_name);
			strcat(tab,"-");// aggiungo un - per indicare che è il direttorio figlio
			if(dirp->d_name[0]!='.'){
				printDirectoryR(fullname,buf,tab);
				tab[strlen(tab)-1]='\0'; // ritorno al padre e tolgo il trattino
			}
		}

	}
	closedir(dp);
	return 1;
}

void scalaEconcatena(char* path,char *nome){
	// Questa funzione serve per rendere il nome ./nome in nome
	// e concatenarla con con il path
	int l=strlen(nome);
	for(int i=0;i<=l-2;i++)
		nome[i]=nome[i+2];
	strcat(path,"/");
	strcat(path,nome);
}

int main(int argc,char *argv[]){
	struct stat buf;
	char path[258];
	if(argc > 2){
		fprintf(stderr,"Errore, parametri inseriti non validi\n");
		exit(-1);
	}
	if(argc==1) // CASO IN CUI NON CI SIA NESSUN ARGOMENTO
		if(getcwd(path,sizeof(path))==NULL){
			fprintf(stderr,"Errore get current working directory");
			exit(-1);
		}
	if(argc==2 && argv[1][0]!='/'){ // CASO IN CUI NON SIA PATH ASSOLUTO 
		// RICAVO IL CURRENT PATH E AGGIUNGO LA CARTELLA
		if(getcwd(path,sizeof(path))==NULL){
			fprintf(stderr,"Errore get current working directory");
			exit(-1);
		}
		if(argv[1][0]=='.')
			scalaEconcatena(path,argv[1]);
		else{
			strcat(path,"/");
			strcat(path,argv[1]);
		}
			
	}else if(argc==2){
		printf("Hai inserito: %s\n",argv[1]);
		strcpy(path,argv[1]);
	}
	// rimuovo lo slash finale in caso ci sia
	if(path[strlen(path)-1]=='/')
		path[strlen(path)-1]='\0';
	// Recupero informazioni sull'inode
	if(lstat(path,&buf)<0){
		fprintf(stderr,"Errore lettura direttorio %s sbagliato\n",path);
		exit(-1);
	}
	//Controllo che la directory inserita sia un direttorio
	if(S_ISREG(buf.st_mode)){
		fprintf(stderr,"Path inserito non è un direttorio\n");
		exit (-1);
	}
	//Ricorsivamente stampo la directory
	char tab[257];// mi serve per spaziare il direttorio padre dal figlio
	tab[0]='\0';
	printf("Stai cercando nel path: %s\n",path);
	if(printDirectoryR(path,&buf,tab)==-1)
		exit(-1);
	
	return 0;
}

