#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/wait.h>

#define BUFF 10

void manager(int sig){
	return;
}

int main(void){
	int pp[2];
	char str[BUFF];
	int n,l;
	pid_t clientPid;
	
	if(pipe(pp)<0){
		fprintf(stderr,"Errore creazione pipe\n");
		exit(1);
	}
	
	signal(SIGUSR1,manager);
	
	if(fork()){
		if(fork()){
			wait(NULL);
			wait(NULL);
		}else{
			//FIGLIO 2 (LETTORE) (CLIENT)
			close(pp[0]); // Chiudo la pipe in lettura
			while(1){
				fprintf(stdout,"Inserisci la stringa da convertire in minuscolo\n");
				//fscanf(stdin,"%s",str);
				fgets(str,BUFF,stdin);
				l=strlen(str);
				str[l-1]='\0';
				l--;
				//fprintf(stdout,"stringa letta: %s\n",str);
				//mando il pid del lettore(serve poi per svegliarlo)
				clientPid=getpid();
				n=write(pp[1],&clientPid,sizeof(pid_t));
				n+=write(pp[1],&l,sizeof(int));
				n+=write(pp[1],str,(l+1)*sizeof(char));
				if(n!=(l+1)*sizeof(char)+sizeof(pid_t)+sizeof(int)){
					fprintf(stderr,"Errore scrittura su pipe\n");
					exit(1);
				}
				if(strcmp(str,"end")==0){
					close(pp[1]);
					exit(0);
				}
				pause();
			}
		}
	}else{
		//FIGLIO 1 (CONVERSORE,STAMPATORE) (SERVER)
		// Tutti i figli successivi conoscono il pid del figlio 1 quindi può essere usato come server
		close(pp[1]); // Chiudo pipe in scrittura
		int len;
		while(1){
			len=0;
			n=read(pp[0],&clientPid,sizeof(pid_t));
			n+=read(pp[0],&l,sizeof(int));
			do{
				len+=read(pp[0],str,BUFF*sizeof(char));
			}while(len<l+1);
			if((n+len)!=(l+1)*sizeof(char)+sizeof(pid_t)+sizeof(int)){
				fprintf(stderr,"Errore lettura pipe\n");
				//exit(1);
			}
			if(strcmp(str,"end")==0){
					close(pp[0]);
					exit(0);
			}
			//conversione in minuscolo
			for(int i=0;i<strlen(str);i++){
				str[i]=(char)tolower(str[i]);
			}
			fprintf(stdout,"Stringa in minuscolo:\n%s\n",str);
			kill(clientPid,SIGUSR1);
		}
	}
	return 0;
}
