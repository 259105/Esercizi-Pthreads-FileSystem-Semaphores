#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//METODO RICORSIVO
void createChildren(int n){ 
  int pid=0;
  if (n) {         
    pid=fork(); 
    if (pid!=0) {
      pid=fork();
      if (pid!=0)
        exit(0);    
    }
    createChildren(n-1);             
  }
  return; 
}

void genBallaEfigli(int b,int e){
	for(int i=0;i<e;i++){ // ESPONENTE
		// Metodo più complesso con base a piacere
		int p;
		for(int j=0;j<b;j++){ // BASE
			p=fork();
			if(!p)
				break;
		}
		if(p)
			exit(0);
		
		/*
		//Metodo semplice con base fissata B=2
		if(fork() && fork()) 
			exit(0);
			*/
	}
}

int main(int argc,char *argv[]){
	int n,t;
	if(argc!=3){
		fprintf(stderr,"Errore parametri inseriti\n");
		exit(1);
	}
	n=atoi(argv[1]);
	t=atoi(argv[2]);
	genBallaEfigli(2,n);
	sleep(t);
	fprintf(stdout,"Terminazione PID=%d.\n",getpid());
	return 0;
}
