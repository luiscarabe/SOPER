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
#include "semaforos.h"

#define FILEKEY "/bin/cat" /*Util para ftok */
#define KEY 1300
#define SEMNUM 5

int main(){

    int semid, key;
    int i;
    int* active;
    unsigned short* init;

    union semun {
        int val;
        struct semid_ds *semstat;
        unsigned short *array;
    } arg;

    key = ftok(FILEKEY, KEY);
    if (key == -1) {
        fprintf (stderr, "Error with key \n");
        return -1;
    }

    /*Comprobacion de Crear_Semaforo*/

    if(Crear_Semaforo(key, SEMNUM, &semid) != 0){
        fprintf(stderr, "Error en Crear_Semaforo(retorno)\n");
        return -1;
    }
    if(semid < 1){
        fprintf(stderr, "Error en Crear_Semaforo(id mal generado)\n");
        return -1;
    }

    arg.array = (unsigned short*)malloc(SEMNUM*sizeof(unsigned int));
    semctl(semid,0, GETALL, arg);
    for(i=0; i<SEMNUM; i++){
        if(arg.array[i] != 0){
            fprintf(stderr, "Error en Crear_Semaforo(no bien inicializado)\n");
            return -1;
        }
    }

    if(Crear_Semaforo(key, SEMNUM, &semid) != 1){
        fprintf(stderr, "Error en Crear_Semaforo(retorno)\n");
        return -1;
    }

    /*Comprobacion de Inicializar_Semaforo*/
    init = (unsigned short*) malloc (SEMNUM*sizeof(unsigned short));
    if(init == NULL){
        return -1;
    }
    for(i=0; i< SEMNUM; i++){
        init[i] = i;
    }

    if(Inicializar_Semaforo(semid, init) == ERROR){
        fprintf(stderr, "Error en Inicializar_Semaforo (Retorno)\n" );
        return -1;
    }

    semctl(semid,0, GETALL, arg);
    for(i=0; i<SEMNUM; i++){
        if(arg.array[i] != i){
            fprintf(stderr, "Error en Inicializar_Semaforo (valores)\n");
            return -1;
        }
    }

    free(init);

    /*Comprobacion de UpMultiple_Semaforo*/
    active = (int*)malloc(SEMNUM*sizeof(int));
    if(active == NULL){
        return -1;
    }
    for(i=0; i<SEMNUM; i++){
        active[i] = i;
    }

    if(UpMultiple_Semaforo(semid, SEMNUM, SEM_UNDO, active) == ERROR){
        fprintf(stderr, "Error en UpMultiple_Semaforo (Retorno)\n" );
        free(active);
        return -1;
    }

    semctl(semid,0, GETALL, arg);


    for(i=0; i < SEMNUM; i++){
        if(arg.array[i] != i+1){
            fprintf(stderr, "Error en UpMultiple_Semaforo (valores)\n");
            free(active);
            return -1;
        }
    }

    /*Comprobacion de DownMultiple_Semaforo*/

    if(DownMultiple_Semaforo(semid, SEMNUM, SEM_UNDO, active) == ERROR){
        fprintf(stderr, "Error en DownMultiple_Semaforo (Retorno)\n" );
        free(active);
        return -1;
    }

    semctl(semid,0, GETALL, arg);
    for(i=0; i<SEMNUM; i++){
        if(arg.array[i] != i){
            fprintf(stderr, "Error en DownMultiple_Semaforo (valores)\n");
            free(active);
            return -1;
        }
    }

    free(active);

    /*Comprobacion de Up_Semaforo*/

    if(Up_Semaforo(semid, 2, SEM_UNDO) == ERROR){
        fprintf(stderr, "Error en Up_Semaforo (Retorno)\n" );
        return -1;
    }
    if(semctl(semid, 2, GETVAL, arg) != 3){
        fprintf(stderr, "Error en Up_Semaforo (Incremento mal realizado)\n" );
        return -1;
    };

    /*Comprobacion del Down_Semaforo*/

    if(Down_Semaforo(semid, 2, SEM_UNDO) == ERROR){
        fprintf(stderr, "Error en Down_Semaforo (Retorno)\n" );
        return -1;
    }
    if(semctl(semid, 2, GETVAL, arg) != 2){
        fprintf(stderr, "Error en Down_Semaforo (Decremento mal realizado)\n" );
        return -1;
    };

    /*Comprobacion de Borrar_Semaforo*/

    if(Borrar_Semaforo(semid) == ERROR){
        fprintf(stderr, "Error en Borrar_Semaforo");
        return -1;
    }

    fprintf(stdout, "No se han producido errores en la prueba de la librería \"semáforos.h\"\n");
    free(arg.array);
    return 0;
}
