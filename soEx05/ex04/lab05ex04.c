#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>

#define SIZEBUFF 1024

void manager (int sig){
	return;
}

void stringUpper(char* s){
	for(int i=0;i<strlen(s);i++)
		s[i]=(char)toupper(s[i]);
}

int main(int argc, char *argv[]){
	setbuf(stdout,0);
	int fd= open("buff",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP);
	if(fd<0){
		fprintf(stderr,"Errore creazione file di buffer\n");
		exit(1);
	}
	// gestore di segnali
	if(signal(SIGUSR1,manager)==SIG_ERR){
	fprintf(stderr,"Errore creazione gestore segnali\n");
		exit(1);
	}
	pid_t server;
	if((server=fork())){
		// PADRE
		if(fork()){
			// PADRE
			// aspetto i 2 figli
			wait(NULL);
			wait(NULL);
		}else{
			// PRODUTTORE
			while(1){
				pid_t my_pid=getpid();
				// chiedo di inserire la stringa da rendere maiuscola
				char s[SIZEBUFF];
				fprintf(stdout,"Inserisci stringa da dare in pasto al consumatore:\n");
				fscanf(stdin,"%s",s);
				//carico sul file attraverso syscall il pid del client e messaggio
				//printf("scrivo\n");
				int wr=write(fd,&my_pid,sizeof(pid_t));
				wr+=write(fd,s,SIZEBUFF*sizeof(char));
				if(wr!=SIZEBUFF*sizeof(char)+sizeof(pid_t)){
					fprintf(stderr,"Errore scrittura su buffer\n");
					//exit(1);
				}
				lseek(fd,0,SEEK_SET);
				// mando segnale per svegliare il consumatore
				//printf("mando segnale\n");
				//sleep(1);
				kill(server,SIGUSR1);
				if(strcmp(s,"end")==0)
					exit(0);
				// vado a dormire
				pause();
			}
		}
	}else{
		//COUNSUMATORE
		while(1){
			// dormo finche non c'è lavoro da fare
			pause();
			char srd[SIZEBUFF];
			pid_t source;
			int rd=read(fd,&source,sizeof(pid_t));
			rd+=read(fd,srd,SIZEBUFF*sizeof(char));
			// leggo sul buffer
			if(rd!=SIZEBUFF*sizeof(char)+sizeof(pid_t)){
				fprintf(stderr,"Errore lettura di buffer\n");
				//exit(1);
			}
			lseek(fd,0,SEEK_SET);
			if(strcmp(srd,"end")==0)
					exit(0);
			// stampo la stringa in maiuscolo
			//fprintf(stdout,"Conversione ... \n");
			stringUpper(srd);
			fprintf(stdout,"%s\n",srd);
			// mando segnale per svegliare il produttore
			//sleep(1);
			kill(source,SIGUSR1);
		}
	}
	close(fd);
	execlp("rm","delete","buff",NULL);
	return 0;
}
