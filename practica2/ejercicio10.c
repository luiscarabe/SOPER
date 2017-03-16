#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <time.h> 

#define TAMANIO 13


void manejador_SIGALRM(int sig){
    if(kill(getppid(), SIGUSR1) == -1){
        perror("Se ha producido un error enviando la señal1");
        exit(EXIT_FAILURE);
    }

    if(signal(SIGALRM,manejador_SIGALRM)==SIG_ERR){
        perror("Error en la captura.\n");
        exit(EXIT_FAILURE);
    }
}

void manejador_SIGUSR1(int sig){
    printf("Senial recibida!\n");
    if(signal(SIGUSR1,manejador_SIGUSR1)==SIG_ERR){
        perror("Error en la captura.\n");
        exit(EXIT_FAILURE);
    }

}


int main(){
    int i, index;
    int pid;
    FILE* f;
    char* frase[]={"EL","PROCESO","A", "ESCRIBE", "EN", "UN", "FICHERO", "HASTA", "QUE", "LEE", "LA", "CADENA", "FIN"};
    char aux[30];
    
    if(signal(SIGUSR1,manejador_SIGUSR1)==SIG_ERR){
        perror("Error en la captura.\n");
        exit(EXIT_FAILURE);
    }

    for(i=0; i<50;i++){

        f=fopen("ficheroe10.txt", "w");
        fclose(f);

        printf("Iteracion %d\n", i+1);

        pid=fork();

        if(pid == -1){
            perror("Error de fork");
            exit(EXIT_FAILURE);
        }

        if(pid == 0){
            
            
            /*srand(getpid());*/
            while(1){
                f=fopen("ficheroe10.txt", "w");
                index=rand()%TAMANIO;
                fprintf(f,"%s ",frase[index]);
                printf("El hijo ha escrito %s\n", frase[index]);                
                    

                if(strcmp(frase[index], "FIN") == 0){
                    fclose(f);
                    exit(EXIT_SUCCESS);
                }
                fclose(f);
            }

        }else{
           
            
           f=fopen("ficheroe10.txt", "r");           

           do{

            fscanf(f,"%s",aux);
            printf("El padre ha leido: %s\n", aux);
            sleep(5);
           
           } while(strcmp(aux,"FIN"));

           fclose(f); 

        }
    }

    if(kill(pid,15)==-1){
        perror("Error enviando senial");
        exit(EXIT_FAILURE);
    }

    wait(NULL);

    exit(EXIT_SUCCESS);

}
