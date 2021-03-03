#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char *argv[]){
	FILE *fs;
	// CONTROLLI INIZIALI E APERTURA FILE
	if(argc!=2){
		fprintf(stderr,"Errore parametri inseriti n=%d",argc);
		exit(1);
	}
	if((fs=fopen(argv[1],"r"))==NULL){
			fprintf(stderr,"Errore apertura file");
			exit(1);
	}
	char currCmd[1024];
	char currArg[256];
	currCmd[0]='\0';
	// SYSTEM
	while(fscanf(fs,"%s",currArg)>0){
		if(!strcmp(currArg,"end")){
		fprintf(stdout,"Eseguo (system) comando: %s :\n",currCmd);
			if(system(currCmd)<0){
				fprintf(stderr,"Errore system\n");
				exit(1);
			}
			currCmd[0]='\0';
			fprintf(stdout,"\n");
			continue;
		}
		strcat(currCmd,currArg);
		strcat(currCmd," ");
	}
	rewind(fs); //SPOSTO IL PUNTATORE ALL'INIZIO
	// EXEC
	char *currCmdV[10]; // comando con un massimo di 10 argomenti
	int nArg=0;
	currArg[0]='\0';
	while(fscanf(fs,"%s",currArg)>0){
		if(!strcmp(currArg,"end")){
			currCmdV[nArg++]=NULL;
			sleep(3);
			if(!fork()){
				if(execvp(currCmdV[0],currCmdV)<0){
					fprintf(stderr,"Errore exec\n");
					exit(1);
				}
			}
			wait(NULL);
			fprintf(stdout,"\n");
			nArg=0;
			continue;
		}
		currCmdV[nArg++]=strdup(currArg);
	}
	fclose(fs);
	exit(0);
}
