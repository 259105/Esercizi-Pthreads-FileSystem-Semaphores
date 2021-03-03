#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char *argv[]){
	printf("P1\n");
	if(fork()){
		printf("P2\n");
		pid_t pidF2;
		if((pidF2=fork())){
			printf("P4\n");
			waitpid(pidF2,(int *)0,0);
		}else{
			//sleep(2);
			printf("P5\n");
			exit(0);
		}
		printf("P7\n");
		wait((int *)0);
	}else{
		//sleep(2);
		printf("P3\n");
		if(fork()){
			wait((int *)0);
		}else{
			//sleep(2);
			printf("P6\n");
			exit(0);
		}
		printf("P8\n");
		exit(0);
	}
	printf("P9\n");
	return(0);
}
