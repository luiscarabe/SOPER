/**
* @brief Programa correspondiente al ejercicio 6 de la practica 1 de sistemas operativos
*
* Este programa se utilizara para estudiar si la comunicacion entre dos procesos es posible
* mediante el uso de punteros.
*
* @file ejercicio6.c
* @author Luis Carabe y Emilio Cuesta
* @date 3-03-2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LENGHT 80

/**
 * @brief Funcion main del programa.
 * @return EXIT_SUCCESS si no ha habido errores, EXIT_FAILURE en caso contrario.
 */
int main (){
    char* cadena;
    int pid;

    /*Reserva de memoria para la cadena de caracteres*/
    cadena=(char*)malloc(LENGHT*sizeof(char));

    pid=fork();
    if(pid<0){
        printf("Error haciendo fork\n");
    exit(EXIT_FAILURE);
    }else if (pid == 0){
        printf("Introduzca un nombre, por favor:\n");
        scanf("%s",cadena);
        /*Se almacena lo escaneado en cadena*/
        fprintf(stdout,"El nombre que que el proceso hijo ha leido es %s\n", cadena);
        exit(EXIT_SUCCESS);
    } else{   
        /*El proceso padre espera a que acabe el hijo, asi nos aseguramos de que ya se ha insertado el nombre*/
        wait(NULL);
    }

    fprintf(stdout,"El nombre que que el proceso padre lee es %s\n", cadena);
    free(cadena);
    exit(EXIT_SUCCESS);
}

/*No podemos ver el resultado correctamente desde el proceso padre, no se por que, yo pensaba que si*/
