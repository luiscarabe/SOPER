/**
* @brief 
* 
* @file 
* @author Luis Carabe y Emilio Cuesta
* @date 
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
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

    void manejador_SIGUSR1 ( );
    void manejador_SIGTERM ( );

    if(argc < 3){
        perror("Error en los pará,etros de entrada.\n");
        exit(EXIT_FAILURE);
    }

    N = atoi(argv[1]);
    V = atoi(argv[2]);

    if(signal(SIGUSR1,manejador_SIGUSR1)==SIG_ERR){
        perror("Error en la captura.\n");
        exit(EXIT_FAILURE);
    }

    if(signal(SIGTERM,manejador_SIGTERM)==SIG_ERR){
        perror("Error en la captura.\n");
        exit(EXIT_FAILURE);
    }

    rootpid = getpid();
    counter = 0;

    pid=0;
    for (i=0; i < N && pid == 0; i++){
        if ((pid=fork()) < 0 ){
            printf("Error haciendo fork\n");
            exit(EXIT_FAILURE);
        }else if (pid == 0){
            if (i == N-1){
                sleep(5);
                if(kill(rootpid,10) == -1){
                    perror("Se ha producido un error enviando la señal");
                    exit(EXIT_FAILURE);
                }
            }
        }else{
            pause();
        }
    }

    exit(EXIT_SUCCESS);
}

void manejador_SIGTERM(int sig){
    sleep(1);
     if(i == N-1){
        if(kill(rootpid,15) == -1){
            perror("Se ha producido un error enviando la señal");
            exit(EXIT_FAILURE);
        }
    }
    else if(kill(pid,15) == -1 && i != 0){
        perror("Se ha producido un error enviando la señal");
        exit(EXIT_FAILURE);
    }
    printf("Muere PID= %d \n", getpid());
    exit(EXIT_SUCCESS);

}

void manejador_SIGUSR1(int sig){
    printf("Hola PID= %d, time= %s \n", getpid(), gettime());
    sleep(2);
    if(i == N-1){
        if(kill(rootpid,10) == -1){
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
        else if(kill(pid,10) == -1){
            perror("Se ha producido un error enviando la señal");
            exit(EXIT_FAILURE);
        }
    }
    else if(kill(pid,10) == -1){
        perror("Se ha producido un error enviando la señal");
        exit(EXIT_FAILURE);
    }

}

char* gettime() {
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char* output;

    output=(char*)malloc(128*sizeof(char));

    strftime(output, 128, "%d/%m/%y %H:%M:%S", tlocal);
    return output;
} 