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
			printf ("PADRE %d\n", i);
            /*Codigo copiado*/
            pid2=fork();
            if(pid2==0){
                pid3 = getppid();
                /*Se convierte el pid en char* para pdoer pasarlo como arg de execlp*/
                sprintf(aux,"%d", pid3);
                printf("\n");
                /*-p incluye los pids en el arbol, al poner el numero este proceso actua como raiz*/
                execlp("pstree", "pstree", aux, "-p", NULL);
                perror("Error en exec");
                exit(EXIT_FAILURE);
            }
            else{
                waitpid(pid2, NULL,0);
            }
            /*Fin del codigo copiado*/
		}
	}
	exit(EXIT_SUCCESS);
}
