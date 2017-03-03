#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_PROC 3

int main (void){
	int pid=0;
	int i;
	printf("PADRE principal, mi pid es el %d\n", getpid());
	for (i=0; i < NUM_PROC && pid == 0; i++){
		if ((pid=fork()) < 0 ){
			printf("Error haciendo fork\n");
			exit(EXIT_FAILURE);
		}else if (pid == 0){
			printf("HIJO con i: %d, mi pid es el %d y el de mi padre el %d.\n", i, getpid(), getppid());
		}else{
			printf ("PADRE %d\n", i);
		}
	}
    wait(NULL);
	exit(EXIT_SUCCESS);
}
