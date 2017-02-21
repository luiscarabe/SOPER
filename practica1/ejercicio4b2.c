#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_PROC 3

int main (void){
	int pid, pid2,pid3;
	int i;
	char buff[10];
	for (i=0; i < NUM_PROC; i++){
		if ((pid=fork()) <0 ){
			printf("Error haciendo fork\n");
			exit(EXIT_FAILURE);
		}else if (pid ==0){
			printf("HIJO con i: %d, mi pid es el %d y el de mi padre el %d.\n", i, getpid(), getppid());
		}else{
			printf ("PADRE %d\n", i);

			/*Codigo copiado*/
			pid2=fork();
			if(pid2==0){
				pid3 = getppid();
				sprintf(buff,"%d", pid3);
				execlp("pstree", "pstree", buff, "-p", NULL);
				exit(EXIT_SUCCESS);
			}
			else{
				wait(NULL);
			}
			/*Fin del codigo copiado*/
		}

	}
	wait(NULL);
	exit(EXIT_SUCCESS);
}
