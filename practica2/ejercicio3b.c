/**
* @brief Programa correspondiente al ejercicio 3b de la practica 2 de sistemas operativos
*
* Este programa servira para generar 100 hilos y que cada uno busque los N primeros primos.
* 
* @file ejercicio3b.c
* @author Luis Carabe y Emilio Cuesta
* @date 17-03-2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

/**
* @brief Boolean (FALSE o TRUE)
*
*/

typedef enum {FALSE=0, TRUE=1} Bool;

/**
* @brief Funcion que nos dice si un número es primo
*
* @param int p: numero para analizar
* @return TRUE en caso de que sea primo o FALSE en caso contrario
*/

Bool es_primo(int p);

/**
* @brief Funcion que especifica el comportamiento de los hilos, en este caso, calcular los primeros N primos
*
* @param N cuantos primos queremos que calcule cada hilo
* @return void*
*/

void* threadbehaviour(void* n);

/**
* @brief Funcion main del programa
*
* @param argv puntero de string que contiene N, es decir, cuantos primos queremos que calcule cada hilo
* @return EXIT_SUCCESS si no ha habido errores, EXIT_FAILURE en caso contrario
*/

int main(int argc, char* argv[]){

    int n;
    int i;
    struct timeval time1, time2;
    double time;
    pthread_t ids[100];

    if (argc != 2){
        perror("Es necesario pasar como argumento el numero de primos a calcular\n");
        exit(EXIT_FAILURE);
    }
    
    n = atoi(argv[1]);

    /*Miramos el tiempo actual y empezamos a crear hilos*/

    gettimeofday(&time1,NULL);

    for(i=0; i < 100; i++){
        pthread_create(&ids[i], NULL, threadbehaviour, (void*) &n);
    }
    
    /*Esperamos a los hilos, medimos el tiempo y calculamos los segundos que han transcurrido*/

    for(i=0; i < 100; i++){
        pthread_join(ids[i], NULL);
    }

    gettimeofday(&time2, NULL);
    time = (time2.tv_sec - time1.tv_sec) + (time2.tv_usec - time1.tv_usec)*1e-6;

    printf("El programa %s termino correctamente, con un tiempo de %f segundos.\n", argv[0], time);
    exit(EXIT_SUCCESS);
}

Bool es_primo(int p){
    int i;
    int s;
    s = sqrt(p);
    for (i = 2; i <= s; i++){
        if ((p % i) == 0){
            return FALSE;
        }
    }
    return TRUE;
}

void* threadbehaviour(void* n){
    int counter;
    int *num, i;

    num = (int*)n; 
    counter = 0;

    for(i=1; counter!=*num; i++){

        if(es_primo(i)){
            counter++;
        }
    }

    pthread_exit(NULL);

}
