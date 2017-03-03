/**
* @brief Programa correspondiente al ejercicio 5a de la practica 1 de sistemas operativos
*
* Este programa se utilizara para generar procesos en serie, es decir, 
* de tal forma que cada proceso tenga tan solo un hijo.
* 
* @file ejercicio5a.c
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
 * @return EXIT_SUCCESS si no habido errores, EXIT_FAILURE en caso contrario.
 */
int main (void){
    int pid;
    int i;

    printf("PADRE principal, mi pid es el %d\n", getpid());
    pid=0;
    for (i=0; i < NUM_PROC && pid == 0; i++){
        if ((pid=fork()) < 0 ){
            printf("Error haciendo fork\n");
            exit(EXIT_FAILURE);
        }else if (pid == 0){
            printf("HIJO con i: %d, mi pid es el %d y el de mi padre el %d.\n", i, getpid(), getppid());
        }else{
            printf ("PADRE %d\n", i);
        }
    }

    wait(NULL);
    exit(EXIT_SUCCESS);
}
