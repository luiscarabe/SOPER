/**
* @brief Programa correspondiente al ejercicio 6 de la practica 2 de sistemas operativos
*
* Este programa servira para generar un proceso que imprima un mensaje cada 5 segundos, 
* a los 30, el programa padre debera enviar una señal para matarle
* 
* @file ejercicio6.c
* @author Luis Carabe y Emilio Cuesta
* @date 17-03-2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/**
* @brief Funcion main del programa
*
* @return EXIT_SUCCESS si no ha habido errores, EXIT_FAILURE en caso contrario
*/

int main(){
    int pid;

    pid=fork();

    if(pid==0){
        pid = getpid();
        while(1){
            /*El hijo imprime y espera*/
            printf("Soy el proceso hijo con pid %d\n", pid);
            sleep(5);
        }
    }
    else{
        /*El padre espera 30 segundos y mata al hijo con una señal*/
        sleep(30);
        if(kill(pid,SIGKILL) == -1){
            perror("Se ha producido un error enviando la señal");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
}
