/**
* @brief 
* 
* @file 
* @author Luis Carabe y Emilio Cuesta
* @date 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> 


int pid, rootpid;
int i;
int N,V;
int counter;

/**
 * @brief Funcion main del programa.
 * @return EXIT_SUCCESS si no habido errores, EXIT_FAILURE en caso contrario.
 */

char* gettime();

int main (int argc , char *argv[]){

    void manejador_SIGUSR1();
    void manejador_SIGTERM();

    if(argc < 3){
        perror("Error en los parametros de entrada.\n");
        exit(EXIT_FAILURE);
    }

    N = atoi(argv[1]);
    V = atoi(argv[2]);

    signal(SIGUSR1,manejador_SIGUSR1);

    rootpid = getpid();
    printf("root1 %d\n", rootpid);
    counter = 0;

    pid=0;
    for (i=0; i < N && pid == 0; i++){

        printf("%d\n", i);
        if ((pid=fork()) < 0 ){
            printf("Error haciendo fork\n");
            exit(EXIT_FAILURE);
        }else if (pid == 0){
            if (i == N-1){
                sleep(5);
                printf("justo antes del kill root %d pid %d\n", rootpid, getpid());
                if(kill(rootpid,16) == -1){
                    perror("Se ha producido un error enviando la señal");
                    exit(EXIT_FAILURE);
                }
            }
        }else{
        }
    }

    wait(NULL);
    exit(EXIT_SUCCESS);
}


void manejador_SIGUSR1(int sig){

    signal(sig,SIG_IGN);

    printf("Hola PID= %d\n", getpid());
    sleep(2);
    if(i == N-1){
        if(kill(rootpid,16) == -1){
            perror("Se ha producido un error enviando la señal");
            exit(EXIT_FAILURE);
        }
    }
    else if(i == 0){
        counter ++;
        if (V == counter){
            if(kill(pid,15) == -1){
                perror("Se ha producido un error enviando la señal");
                exit(EXIT_FAILURE);
            }
        }
        else if(kill(pid,16) == -1){
            perror("Se ha producido un error enviando la señal");
            exit(EXIT_FAILURE);
        }
    }
    else if(kill(pid,16) == -1){
        perror("Se ha producido un error enviando la señal");
        exit(EXIT_FAILURE);
    }

    signal(sig,manejador_SIGUSR1);


}

