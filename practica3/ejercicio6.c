/**
* @brief Programa correspondiente al ejercicio 6 de la práctica 3 de Sistemas Operativos
*
* Este programa coordina el uso de memoria compartida y de semaforos para garantizar una
* buena comunicacion entre ambos. Se trata de una representacion del clasico problema de
* productor - consumidor. En este caso, el proceso productor generara en orden las letras
* del abecedario y las ira almecenando en un array. Por su parte, el proceso hijo ira leyendo
* dichas letras de cada posicion del array siempre y cuando ya hayan sido escritas.
*
* @file ejercicio6.c
* @author Luis Carabe y Emilio Cuesta
* @date 04-04-2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "semaforos.h"

#define FILEKEY "/bin/cat" /*Util para ftok */
#define KEY 1300


/**
* @brief Estructura guardada en la region de memoria compartida.
*
* Esta estructura dispone de un campo: una cadena de caracteres que va almacenando
* progresivamente el alfabeto
*/
typedef struct _buffer{
  char alfb[26];
} shm;


/*
* @brief Esta funcion simula el comportamiento del productor. Comprueba que la memoria
* no esta siendo accedida por otro proceso, luego, guarda en el array la letra pertinente
* y sale de la zona de memoria compartida. En el caso de ser la primera letra la que se
* almacena, pone a 1 el semaforo que controla este aspecto.
*/
void productor();

/*
* @brief Esta funcion simula el comportamiento del consumidor. Comprueba que la memoria
* no esta siendo accedida por otro proceso, luego, lee del array la letra pertinente,
* la imprime por pantalla y sale de la zona de memoria compartida. En el caso de ser la
* primera letra la que se lee, tendra que esperar a que el semaforo que controla esta
* situacion se active.
*/
void consumidor();

/*
*@brief Funcion main del programa, crea una zona de memoria compartida y dos semaforos
* para coordinar al productor y al consumidor. Uno de los semaforos servira para asegurarse
* de que el consumidor no imprima antes de que el productor haya guardado una variable por
* primera vez. El otro servira para evitar problemas de acceso simultaneo a la memoria compartida.
* Posteriormente, realiza un fork(), el proceso padre toma la labor del consumidor y el proceso
* hijo la del productor. Para esto, se utilizan las funciones productor() y consumidor() definidas
* en este mismo fichero. Una vez se ha impreso todo el abecedario por pantalla, el programa finaliza.
*/
int main(){
    int pid;
    int key1, key2;
    int id_zone;
    shm* buffer;
    int semid;
    unsigned short init[2];

    key1 = ftok(FILEKEY, KEY);
    if (key1 == -1) {
        fprintf (stderr, "Error with key \n");
        return -1;
    }

    key2 = ftok(FILEKEY, KEY);
    if (key2 == -1) {
        fprintf (stderr, "Error with key \n");
        return -1;
    }
    /*Inicializacion de la memoria compartida*/
    id_zone = shmget(key1, sizeof(shm), IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
    if( id_zone == -1){
        fprintf(stderr, " 1 Error with id_zone \n");
        exit(EXIT_FAILURE);
    }

    buffer = shmat( id_zone, (char*) 0, 0);
    if(buffer == NULL){
        fprintf(stderr, "Error reserve shared memory \n");
        exit(EXIT_FAILURE);
    }

    /*Creacion e inicializacion de los semaforos*/
    if(Crear_Semaforo(key2, 2, &semid) != 0){
        fprintf(stderr, "Error en Crear_Semaforo");
        return -1;
    }

    init[0] = 0;
    init[1] = 1;
    if(Inicializar_Semaforo(semid,init) == ERROR){
        fprintf(stderr, "Error en Inicializar_Semaforo");
        return -1;
    }

    pid=fork();
    if(pid < 0){
        fprintf(stderr, "Se ha producido un error en el fork");
        return -1;
    }
    else if(pid > 0){

        consumidor(buffer, semid);
        wait(NULL);

    }
    else if (pid == 0){

        /*buffer = shmat( id_zone, (char*) 0, 0);
        if(buffer == NULL){
            fprintf(stderr, "Error reserve shared memory \n");
            exit(EXIT_FAILURE);
        }*/
        productor(buffer, semid);
        shmdt ((char *)buffer);
        exit(EXIT_SUCCESS);
    }


    shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
    Borrar_Semaforo(semid);
    return 0;
}

void productor(shm* buffer, int semid){
    int i, flag = 0;
    for(i=65; i<=90; i++){
        if(Down_Semaforo(semid, 1, SEM_UNDO) != OK){
            fprintf(stderr, "Error en Down_Semaforo\n");
            return;
        }
        buffer->alfb[i] = (char) i;
        if(Up_Semaforo(semid, 1, SEM_UNDO) != OK){
            fprintf(stderr, "Error en Up_Semaforo\n");
            return;
        }
        if(flag == 0){
            if(Up_Semaforo(semid, 0, SEM_UNDO) != OK){
                fprintf(stderr, "Error en Up_Semaforo\n");
                return;
            }
            flag = 1;
        }
    }
    return;
}

void consumidor(shm* buffer, int semid){
    int i, flag = 0;
    for(i=65; i<=90; i++){
        if(flag == 0){
            if(Down_Semaforo(semid, 0, SEM_UNDO) != OK){
                fprintf(stderr, "Error en Down_Semaforo\n");
                return;
            }
            flag = 1;
        }
        if(Down_Semaforo(semid, 1, SEM_UNDO)!= OK){
            fprintf(stderr, "Error en Down_Semaforo\n");
            return;
        }
        fprintf(stdout, "Letra %d: %c\n", i-64, buffer->alfb[i]);
        if(Up_Semaforo(semid, 1 ,SEM_UNDO) != OK){
            fprintf(stderr, "Error en Up_Semaforo\n");
            return;
        }
    }
    return;
}
