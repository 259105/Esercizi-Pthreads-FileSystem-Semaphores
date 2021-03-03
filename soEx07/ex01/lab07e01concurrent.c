#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void binaryC(int*,int);
void binaryR(int *, int, int);

int main(int argc, char **argv){
	if(argc!=2){
		fprintf(stderr,"Errore paramentri inseriti non validi:\ninserire solo numero di cifre binarie\n");
		exit(1);
	}
	int n=atoi(argv[1]);
	int *vet=malloc(sizeof(int)*n);
	if(vet==NULL){
		fprintf(stderr,"Allocation Error\n");
		exit(1);
	}
	printf("Binanry Numbers:\n");
	fflush(stdout);
	binaryC(vet,n);
	free(vet);
	return 0;
}

void binaryC(int *vet,int n){
	// SOLUZIONE CONCORRENTE
	pid_t pids[2];
	for(int i=0;i<n;i++){
		if((pids[0]=fork())){
			if((pids[1]=fork())){
				//PADRE dei progessi
				waitpid(pids[1],NULL,0);
				waitpid(pids[0],NULL,0);
				exit(0);
			}
			// FIGLIO 2 (0)
			vet[i]=0;
		}else{
			//FIGLIO (1)
			vet[i]=1;
		}
	}
	for (int j=0; j<n; j++) {
		printf("%d", vet[j]);
  }
  printf("\n");
  exit(0);
}

void
binaryR (
  int *vet,
  int n,
  int i
  )
{
	// SOLUZONE RICORSIVA
  int j;

  if (i >= n) {
    for (j=0; j<n; j++) {
      printf("%d", vet[j]);
    }
    printf("\n");
    return;
  }

  vet[i] = 0;
  binaryR(vet, n, i+1);

  vet[i] = 1;
  binaryR(vet, n, i+1);

  return;
}
