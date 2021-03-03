#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
	/*
		ALLOCAZIONE IN MODO DINAMICA DEI COMANDI CON
		CON UN MAX DI 10 COMANDI E PER OGNI COMANDO CON UN MAX DI 10 ARGC
		OGNI COMANDO PUÒ ESSERE AL MASSIMO LUNGO 256 CHAR
	*/
	char **cmds[10]; //max 10 cmd
	cmds[0]=malloc(10*sizeof(char*)); // max 10 argc for cmd
	int ncmds=0; // indica il numero di comandi
	char currCmd[256]; //current cmd
	int nargc=0;
	
	while(fscanf(fs,"%s",currCmd)>0){
		if(strcmp(currCmd,"end")==0){
			ncmds++;
			nargc=0;
			cmds[ncmds]=malloc(10*sizeof(char*)); // max 10 argc for cmd
			continue;
		}
		cmds[ncmds][nargc++]=strdup(currCmd);
	}
	fclose(fs);
	// FACCIO STAMPA DEI COMANDI INSERITI
	fprintf(stdout,"Hai inserito i seguenti comandi:\n");
	for(int i=0;i<ncmds;i++){
		for(int j=0;;j++){
			if(cmds[i][j]!=NULL)
				fprintf(stdout,"%s ",cmds[i][j]);
			else
				break;
		}
		fprintf(stdout,"\n");
	}
	fprintf(stdout,"\n");
	//ESEGUO ESERCIZIO ATTRAVERSO LA SYSTEM
	for(int i=0;i<ncmds;i++){
		currCmd[0]='\0';
		for(int j=0;;j++){
			if(cmds[i][j]!=NULL){
				strcat(currCmd,cmds[i][j]);
				strcat(currCmd," ");
			}
			else
				break;
		}
		fprintf(stdout,"Eseguo (system) : %d° comando := %s\n",i,cmds[i][0]);
		if(system(currCmd)<0){
			fprintf(stderr,"Errore system sul %d° comando\n",i);
			exit(1);
		}
		fprintf(stdout,"\n");
	}
	fprintf(stdout,"\n");
	// ESEGUO ESERCIZIO ATTRAVERSO L'EXEC
	/* USO LA EXECVP PERCHÉ CONSENTE DI INSERIRE UN NUMERO INDEFINITO DI ARGOMENTI
		 A RUN TIME (MOLTO PIÙ AGEVOLE)
		 E PERCHE COSI POSSO EVITARMI IL PROBLEMA DEL PATH
	*/
	for(int i=0;i<ncmds;i++){
		sleep(3);
		fprintf(stdout,"Eseguo (exec) : %d° comando := %s\n",i,cmds[i][0]);
		if(!fork()){
			//IL FIGLIO CHIAMA EXEC E MUORE
			if(execvp(cmds[i][0],cmds[i])<0){
				fprintf(stderr,"Errore execvp sul %d° comando\n",i);
				exit(1);
			}
		}
	}
	exit(0);
}
