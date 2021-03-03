#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(void){
	int ppP[2];
	int ppF[2];
	int n;
	char s=' ';
	pipe(ppP); pipe(ppF); //creo le pipe

	if(fork()){
		//PADRE
		for(int i=0;i<5;i++){
			fprintf(stdout,"I'm the father (PID=%d)\n",getpid());
			//scrivo sulla pipe del padre per sbloccare il figlio
			n=write(ppP[1],&s,sizeof(char));
			if(n!=sizeof(char)){
				fprintf(stderr,"Errore sincronizzazione\n");
				exit(1);
			}
			// mi blocco con read finche il figlio non scrive sulla sua pipe
			n=read(ppF[0],&s,sizeof(char));
			if(n!=sizeof(char)){
				fprintf(stderr,"Errore sincronizzazione\n");
					exit(1);
			}
		}
		s='e';
		n=write(ppP[1],&s,sizeof(char));
		wait(NULL);
	}else{
		//FIGLIO 
		while(1){
			//mi blocco con read finche il padre non scrive sulla sua pipe
			n=read(ppP[0],&s,sizeof(char));
			if(s=='e'){
				exit(1);
			}
			if(n!=sizeof(char)){
				fprintf(stderr,"Errore sincronizzazione\n");
				exit(1);
			}
			fprintf(stdout,"I'm the child (PID=%d)\n",getpid());
			// scrivo sulla pipe del figlio per sbloccare il padre
			n=write(ppF[1],&s,sizeof(char));
			if(n!=sizeof(char)){
				fprintf(stderr,"Errore sincronizzazione\n");
				exit(1);
			}
		}
	}
	return 0;
}
