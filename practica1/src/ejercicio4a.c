/**
* @brief Programa correspondiente al ejercicio 4a de la practica 1 de sistemas operativos
*
* Este programa se utilizara para estudiar si se generan procesos hijo que se puedan quedar huerfanos.
* 
* @file ejercicio4a.c
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

    for (i=0; i < NUM_PROC; i++){
        if ((pid=fork()) < 0 ){
            printf("Error al emplear fork\n");
            exit(EXIT_FAILURE);
        } else if (pid == 0){
            printf("HIJO %d con pid %d y ppid %d.\n", i, getpid(), getppid());
        } else {
            printf ("PADRE %d con pid %d\n", i, getpid());
        }
    }

    exit(EXIT_SUCCESS);
}
