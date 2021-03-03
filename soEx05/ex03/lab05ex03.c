#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void manager(int sig){
	// GESTIONE DEI SINGOLI SEGNALI, MA IN QUESTO CASO NON SERVE
	return;
}

int main(int argc,char *argv[]){
	if(signal(SIGUSR1,manager)==SIG_ERR){
		fprintf(stderr,"Errore gestore segnali");
		exit(1);
	}
	pid_t pid=fork();
	while(1){
		if(pid){
			pause();
			fprintf(stdout,"Father Woke-up\n");
			sleep(1);
			kill(pid,SIGUSR1);
		}else{
			fprintf(stdout,"Child Woke-up\n");
			sleep(1);
			kill(getppid(),SIGUSR1);
			pause();
		}
	}
	return 0;
}
