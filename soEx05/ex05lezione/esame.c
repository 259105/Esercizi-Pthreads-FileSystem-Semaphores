#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFF 258

void inverti(char *s){
	char c;
	int l=strlen(s);
	for(int i=0;i<(l/2);i++){
		c=s[i];
		s[i]=s[l-1-i];
		s[l-1-i]=c;
	}
}

int main(void){
	int pp1[2]; // TRA LETTORE E SERVER
	int pp2[2]; // TRA SERVER E STAMPANTE
	// CREO PIPE
	setbuf(stdout,0);
	char str[BUFF]; // contiene la string
	int n; // per fare controllo su write e read
	if(pipe(pp1)<0){
		fprintf(stderr,"Errore creazione pipe\n");
		exit(1);
	}
	if(fork()){
		//PADRE (SERVER)
		// CHIUDO WRITE PIPE1
		close(pp1[1]);
		// CREO PIPE
		if(pipe(pp2)<0){
			fprintf(stderr,"Errore creazione pipe\n");
			exit(1);
		}
		if(fork()){
			//PADRE(SERVER)
			//CHIUDO READ PIPE2
			close(pp2[0]);
			n=read(pp1[0],str,BUFF*sizeof(char));
			if(n!=BUFF){
				fprintf(stderr,"Errore lettura string ricevuta da lettore\n");
				exit(1);
			}
			// faccio inversione
			inverti(str);
			// mando la stringa invertita all printer
			n=write(pp2[1],str,BUFF*sizeof(char));
			if(n!=BUFF){
				fprintf(stderr,"Errore durante la spedizione al printer\n");
				exit(1);
			}
			close(pp1[0]);
			close(pp2[1]);
			wait(NULL);
			wait(NULL);		
		}else{
			//FIGLIO 2 (SCRITTORE)
			//CHIUDO WRITE PIPE2
			close(pp2[1]);
			// CHIUDO READ PIPE1 PERCHE NON MI INTERESSA
			close(pp1[0]);
			n=read(pp2[0],str,BUFF*sizeof(char));
			if(n!=BUFF){
				fprintf(stderr,"Errore lettura string ricevuta da server\n");
				exit(1);
			}
			fprintf(stdout,"String invertita:\n%s\n",str);
			close(pp2[0]);
			exit(0);
		}
	}else{
		//FIGLIO 1 (LETTORR)
		// CHIUDO READ PIPE1
		close(pp1[0]);
		fprintf(stdout,"Inserisci stringa da invertire:\n");
		fscanf(stdin,"%s",str);
		// mando la stringa
		n=write(pp1[1],str,BUFF*sizeof(char));
		if(n!=BUFF){
			fprintf(stderr,"Errore durante la spedizione al server\n");
			exit(1);
		}
		// CHIUDO WRITE PIPE1
		close(pp1[1]);
		exit(0);
	}
	return 0;
}
