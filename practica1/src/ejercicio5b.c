/**
* @brief Programa correspondiente al ejercicio 5b de la practica 1
*
* Este programa se utilizara para generar proceso que tenga tres hijos, 
* ninguno de los cuales tendra mas hijos.
* 
* @file ejercicio5b.c
* @author Luis Carabe y Emilio Cuesta
* @date 3-03-2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_PROC 3

/**
 * @brief Funcion main del programa.
 * @return EXIT_SUCCESS si no ha habido errores, EXIT_FAILURE en caso contrario.
 */
int main (void){
    int pid;
    int i;

    printf("PADRE principal, mi pid es el %d\n", getpid());
    pid=1;
    for (i=0; i < NUM_PROC && pid > 0; i++){
        if ((pid=fork()) < 0 ){
            printf("Error haciendo fork\n");
            exit(EXIT_FAILURE);
        }else if (pid == 0){
            printf("HIJO con i: %d, mi pid es el %d y el de mi padre el %d.\n", i, getpid(), getppid());
            exit(EXIT_SUCCESS);
	    }else{
            printf ("PADRE %d\n", i);
        }
    }

    /*Esperamos por cada uno de los procesos*/
    for(i=0;i<NUM_PROC;i++){
        wait(NULL);
    }

    exit(EXIT_SUCCESS);
}
