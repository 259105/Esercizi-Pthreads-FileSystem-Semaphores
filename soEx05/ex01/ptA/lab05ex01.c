#include <stdio.h>
#include <stdlib.h>

int main(void){
	int n;
	while(fscanf(stdin,"%d",&n)==1){
		if(n%2)
			fprintf(stderr,"%d\n",n);
		else
			fprintf(stdout,"%d\n",n);
	}
	return 0;
}
