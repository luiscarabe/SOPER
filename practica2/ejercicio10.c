#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <time.h> 

#define TAMANIO 13

int main(){
    int i, index;
    int pid;
    FILE* f;
    char* frase[]={"EL","PROCESO","A", "ESCRIBE", "EN", "UN", "FICHERO", "HASTA", "QUE", "LEE", "LA", "CADENA", "FIN"};
    char aux[30];

    for(i=0; i<50 && pid!=0 ;i++){
        pid=fork();
        if(pid == -1){

            perror("Error en fork");
            exit(EXIT_FAILURE);

        }else if(pid == 0){
            
            f=fopen("ficheroe10.txt", "a");
            
            while(1){
                index=rand()%TAMANIO;
                fprintf(f,"%s ",frase[index]);
                if(strcmp(frase[index], "FIN") == 0){
                    fclose(f);
                    exit(EXIT_SUCCESS);
                }
                sleep(5);
            }

        }else{

            f=fopen("ficheroe10.txt", "r");

            while(strcmp(aux,"FIN")){
                sleep(5);
                fscanf(f,"%s", aux);
                printf("El padre ha leido : %s", aux);
            }

            fclose(f);

        }
    }

    if(kill(pid,15)==-1){
        perror("Error enviando senial");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

}