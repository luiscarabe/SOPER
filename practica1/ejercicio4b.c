#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_PROC 3

int main (void){
	int pid;
	int i;
	for (i=0; i < NUM_PROC; i++){
		if ((pid=fork()) <0 ){
			printf("Error haciendo fork\n");
			exit(EXIT_FAILURE);
		}else if (pid ==0){
			printf("HIJO con i: %d, mi pid es el %d y el de mi padre el %d.\n", i, getpid(), getppid());
		}else{
			printf ("PADRE %d\n", i);
		}
		system("pstree");
	}
	wait(NULL);
	exit(EXIT_SUCCESS);
}
