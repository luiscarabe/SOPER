#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define NUM_PROC 3

int main (void){
	int pid, pid2, pid3;
	int i;
  char aux[10];
	for (i=0; i < NUM_PROC; i++){
		if ((pid=fork()) < 0 ){
			printf("Error al emplear fork\n");
			exit(EXIT_FAILURE);
		} else if (pid == 0){
			printf("HIJO con i: %d, mi pid es el %d y el de mi padre el %d.\n",i, getpid(), getppid());
		} else {
			printf ("PADRE %d con pid %d\n", i, getpid());
		}
	}
	exit(EXIT_SUCCESS);
}
