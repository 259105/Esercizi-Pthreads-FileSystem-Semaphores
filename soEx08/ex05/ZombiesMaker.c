#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
	if(fork()){
		//Padre di tutti
		while(1){
			sleep(1);
		}
	}else{
		//Figlio del padre
		if(fork()){
			// Figlio esce senza aspettare
			exit(0);
		}else{
			exit(0);
		}
	}
}
