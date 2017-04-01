/**
* @brief Programa correspondiente al ejercicio 4a de la practica 2 de sistemas operativos
*
* @file ejercicio4a.c
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
#include "time.h"

/**
* @brief Estructura de datos de un hilo
*
* Esta estructura define todos los datos que debe manejar un hilo.
*/

typedef struct _args{
    int dim;
    int** matriz;
    int mult;
    int id;
} args;


/**
 * @brief Solicita por pantalla los datos necesarios para rellenar una matriz de dimension de 
 * 0 a 4 para la que reserva memoria y la guarda en la estructura que se pasa como argumento.
 * @param dim Dimension de la matriz a crear.
 * @param operacion Se utiliza para almacenar los datos obtenidos por pantalla y devolverselo al main.
 * @return 0 si todo ha ido bien, -1 en caso de error.
 */
int crear_matriz(int dim, args* operacion);

/**
 * @brief Define el comportamiento de un thread. Recorre la matriz en la estructura n y va multiplicaando
 * fila a fila por el valor que se indica en la misma esructura. Imprime un mensaje tras cada operacion
 * con el resultado y el numero de fila multiplicada. 
 * @param n Estructura de datos que se utiliza para pasar argumentos a esta funcion y ademas para 
 * compartir datos con el proceso padre.
 * @return Sin retorno.
 */
void* threadbehaviour(void* n);

/**
 * @brief Funcion main del programa. Solicita por pantalla una dimension de matriz y luego llama 
 * a la funcion crear_matriz. Posteriormente genera dos threads para realizar las operaciones pertinentes.
 * de multiplicacion de matrices. Antes de finalizar, espera a que los dos hilos generados acaben. 
 * @return EXIT_SUCCESS (todo ha ido bien) o EXIT FAILURE (algo ha fallado).
 */
int main(int argc, char* argv[]){
    int dim;
    int i;
    pthread_t hilo[2];
    args* operacion1;
    args* operacion2;

    do{
        printf("Introduzca dimension de la matriz cuadrada:\n");
        scanf("%d", &dim);

        if (dim <0 || dim > 4){
            printf("La dimension de la matriz no debe exceder 4 o ser menor que 1.\n");
        }

    } while(dim > 4);

    /*Inicializacion de las esructuras de los hilos. Como estos dos hilos no necesitan compartir 
    informacion (en este apartado), las estructura no se comunican.*/
    operacion1 = (args*)malloc(sizeof(args));
    if(operacion1 == NULL){
        exit(EXIT_FAILURE);
    }
    operacion1->dim = dim;
    operacion1->id = 1;

    operacion2=(args*)malloc(sizeof(args));
    if(operacion2 == NULL){
        exit(EXIT_FAILURE);
    }
    operacion2->dim = dim;
    operacion2->id = 2;


    printf("Introduzca multiplicador 1:\n");
    scanf("%d", &operacion1->mult);

    printf("Introduzca multiplicador 2:\n");
    scanf("%d", &operacion2->mult);

    printf("Introduzca matriz 1:\n");
    crear_matriz(dim, operacion1);
    
    printf("Introduzca matriz 2:\n");
    crear_matriz(dim, operacion2);

    printf("Realizando producto:\n");

    /*Creacion de hilos*/
    pthread_create(&hilo[0], NULL, threadbehaviour, (void*) operacion1);
    pthread_create(&hilo[1], NULL, threadbehaviour, (void*) operacion2);

    /*Espera de hilos*/
    pthread_join(hilo[0], NULL);
    pthread_join(hilo[1], NULL);

    /*Liberando recursos*/
    for(i=0; i<dim; i++){
        free(operacion1->matriz[i]);
        free(operacion2->matriz[i]);
    }

    free(operacion1->matriz);
    free(operacion2->matriz);
    free(operacion1);
    free(operacion2);

    printf("El programa %s termino correctamente. \n", argv[0]);
    exit(EXIT_SUCCESS);
}


int crear_matriz(int dim, args* operacion){
    int i;

    if(dim < 0 || dim > 4 || operacion == NULL){
        return -1;
    }

    operacion->matriz = (int**)malloc(dim*sizeof(int*));
    if(!operacion->matriz){
        return -1;
    }

    for(i=0; i<dim; i++){
        operacion->matriz[i] = (int*)malloc(dim*sizeof(int));
        if(!operacion->matriz[i]){
        return -1;
        }
    }
   
    switch(dim){
        case 1:
            break;
            scanf("%d", &operacion->matriz[0][0]);
        case 2:
            scanf("%d %d %d %d", &operacion->matriz[0][0], &operacion->matriz[0][1], 
                                 &operacion->matriz[1][0], &operacion->matriz[1][1]);
            break;
        case 3:
            scanf("%d %d %d %d %d %d %d %d %d", &operacion->matriz[0][0], &operacion->matriz[0][1], &operacion->matriz[0][2], 
                                                &operacion->matriz[1][0], &operacion->matriz[1][1], &operacion->matriz[1][2], 
                                                &operacion->matriz[2][0], &operacion->matriz[2][1], &operacion->matriz[2][2]);
            break;
        default:
            scanf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &operacion->matriz[0][0], &operacion->matriz[0][1], &operacion->matriz[0][2], &operacion->matriz[0][3], 
                                                                     &operacion->matriz[1][0], &operacion->matriz[1][1], &operacion->matriz[1][2], &operacion->matriz[1][3], 
                                                                     &operacion->matriz[2][0], &operacion->matriz[2][1], &operacion->matriz[2][2], &operacion->matriz[2][3], 
                                                                     &operacion->matriz[3][0], &operacion->matriz[3][1], &operacion->matriz[3][2], &operacion->matriz[3][3]);
            break;
    }

    return 0;
}

void* threadbehaviour(void* n){
    int i, j;
    int resultado;
    args* datos = (args*) n;


    for(i=0;i<datos->dim;i++){
        
        printf("Hilo %d multiplicando fila %d resultado", datos->id,i);
        
        for(j=0;j<datos->dim;j++){
            resultado=datos->matriz[i][j]*datos->mult;
            printf(" %d", resultado);
        }

        printf("\n");
        usleep(1000000);
    }


    pthread_exit(NULL);
}
