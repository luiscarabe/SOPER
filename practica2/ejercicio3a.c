/**
* @brief Programa correspondiente al ejercicio 3a de la practica 2 de sistemas operativos
*
* Este programa servira para generar 100 procesos y que cada uno busque los N primeros primos.
* 
* @file ejercicio3a.c
* @author Luis Carabe y Emilio Cuesta
* @date 17-03-2017
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
* @brief Boolean (FALSE o TRUE)
*
*/

typedef enum {FALSE=0, TRUE=1} Bool;

/**
* @brief Funcion que nos dice si un numero es primo
*
* @param p numero para analizar
* @return TRUE en caso de que sea primo o FALSE en caso contrario
*/

Bool es_primo(int p);

/**
* @brief Funcion main del programa
*
* @param argv puntero de string que contiene N, es decir, cuantos primos queremos que calcule cada proceso
* @return EXIT_SUCCESS si no ha habido errores, EXIT_FAILURE en caso contrario
*/

int main(int argc , char *argv[]) {
    int pid;
    int i, counter;
    int n;
    struct timeval time1, time2;
    double time;

    if (argc != 2){
        printf("Error en los argumentos de entrada.\n");
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);
    
    /*Miramos el tiempo actual y empezamos a crear procesos*/

    gettimeofday(&time1, NULL);

    for(i=0; i < 100; i++){
        pid = fork();
        if(pid == -1){
            printf("Error en el fork");
            exit(EXIT_FAILURE);
        }
        else if(pid == 0){
            for(counter = 0, i = 2; counter != n; i++){
                if (es_primo(i)){
                    counter++;
                }
            }
            exit(EXIT_SUCCESS);
        }
    }

    /*Esperamos a los hijos*/

    for(i=0; i < 100; i++){
        wait(NULL);
    }

    /*Volvemos a mirar el tiempo y calculamos los segundos que han transcurrido*/

    gettimeofday(&time2, NULL);

    time = ((time2.tv_sec*1e6 + time2.tv_usec) - (time1.tv_sec*1e6 + time1.tv_usec))*1e-6;



    printf("El programa %s termino correctamente, con un tiempo de %f segundos.\n", argv[0], time);

    exit(EXIT_SUCCESS);
}

Bool es_primo(int p){
    int i;
    double s;
    s = sqrt(p);
    for (i = 2; i <= s; i++){
        if ((p % i) == 0){
            return FALSE;
        }
    }
    return TRUE;
} 
