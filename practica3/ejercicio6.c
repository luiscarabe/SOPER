/**
* @brief Programa correspondiente al ejercicio 6 de la práctica 3 de Sistemas Operativos
*
* Este programa coordina el uso de memoria compartida y de semaforos para garantizar una
* buena comunicacion entre dos procesos de ejecucion paralela.
* Se trata de una representacion del clasico problema de productor - consumidor. En este caso,
* el proceso productor generara en orden las letras del abecedario y las ira almecenando en un array.
* Por su parte, el proceso consumidor ira leyendo dichas letras de cada posicion del array siempre
* y cuando ya hayan sido escritas.
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
* y sale de la zona de memoria compartida. Además, despues de escribir cada letra,
* incrementa el valor del segundo semaforo utilizado para que el proceso consumidor
* sepa que ya tiene algo que leer.
*/
void productor();

/*
* @brief Esta funcion simula el comportamiento del consumidor. Comprueba que la memoria
* no esta siendo accedida por otro proceso y que tiene algo que leer, luego, lee del array
* la letra pertinente, la imprime por pantalla y sale de la zona de memoria compartida. En
* todo momento, si el consumidor ya ha leido todo lo que el padre ha producido, este proceso
* estará bloqueado.
*/
void consumidor();

/*
*@brief Funcion main del programa, crea una zona de memoria compartida y dos semaforos
* para coordinar al productor y al consumidor. Uno de los semaforos servira para asegurarse
* de que el consumidor no lea de memoria antes de que el productor haya escrito algo nuevo.
* El otro servira para evitar problemas de acceso simultaneo a la memoria compartida.
* Posteriormente, se realizan dos fork(), el proceso padre no es ni el consumidor ni el productor.
* Son sus dos hijos los que realizan estas labores para que la ejecucion sea paralela.
* Para esto, se utilizan las funciones productor() y consumidor() definidas en este mismo fichero.
* Una vez se ha creado, leido e impreso todo el abecedario por pantalla, el programa finaliza.
*/
int main(){
    int pid;
    int i;
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

    /*Despues de esto, buffer apunta a la memoria compartida*/
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

    /*Este semaforo sera el que indica si el consumidor tiene alguna letra que leer*/
    init[0] = 0;
    /*Este semaforo se encarga de proteger la memoria compartida*/
    init[1] = 1;
    if(Inicializar_Semaforo(semid,init) == ERROR){
        fprintf(stderr, "Error en Inicializar_Semaforo");
        return -1;
    }

    /*Primer fork() del main*/
    pid=fork();
    if(pid < 0){
        fprintf(stderr, "Se ha producido un error en el fork");
        return -1;
    }
    else if(pid > 0){
        /*Segundo fork() del main*/
        pid=fork();
        if(pid < 0){
            fprintf(stderr, "Se ha producido un error en el fork");
            return -1;
        }
        else if (pid == 0){
            /*Codigo del segundo hijo, el consumidor*/
            consumidor(buffer, semid);
            shmdt ((char *)buffer);
            exit(EXIT_SUCCESS);
        }
        else{
            wait(NULL);
        }

        wait(NULL);

    }
    else if (pid == 0){
        /*Codigo del primer hijo, el productor*/
        productor(buffer, semid);
        shmdt ((char *)buffer);
        exit(EXIT_SUCCESS);
    }

    /*Se borra la memoria compartida y el set se semaforos utilizados*/
    shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
    Borrar_Semaforo(semid);
    return 0;
}

void productor(shm* buffer, int semid){
    int i, flag = 0;
    for(i=65; i<=90; i++){

        /*Comprueba si puede acceder a memoria compartida*/
        if(Down_Semaforo(semid, 1, 0) != OK){
            fprintf(stderr, "Error en Down_Semaforo\n");
            return;
        }
        /*Seccion critica, escritura de ltras*/
        buffer->alfb[i] = (char) i;
        /*Indica que sale de memoria compartida*/
        if(Up_Semaforo(semid, 1, 0) != OK){
            fprintf(stderr, "Error en Up_Semaforo\n");
            return;
        }
        /*Incrementa el contador de letras que el hijo todavia no ha leido*/
        if(Up_Semaforo(semid, 0, 0) != OK){
            fprintf(stderr, "Error en Up_Semaforo\n");
            return;
        }


    }
    return;
}

void consumidor(shm* buffer, int semid){
    int i;
    for(i=65; i<=90; i++){

        /*Comprueba si hay una nueva letra que leer*/
        if(Down_Semaforo(semid, 0, 0) != OK){
                fprintf(stderr, "Error en Down_Semaforo\n");
                return;
        }
        /*Comprueba si puede acceder a la memoria compartida*/
        if(Down_Semaforo(semid, 1, 0)!= OK){
            fprintf(stderr, "Error en Down_Semaforo\n");
            return;
        }
        /*Seccion Critica*/
        fprintf(stdout, "Letra %d: %c\n", i-64, buffer->alfb[i]);
        /*Indica que deja de utilizar la memoria compartida*/
        if(Up_Semaforo(semid, 1 ,0) != OK){
            fprintf(stderr, "Error en Up_Semaforo\n");
            return;
        }
    }
    return;
}
