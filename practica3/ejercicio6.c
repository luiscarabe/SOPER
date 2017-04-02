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

typedef struct _buffer{
  char alfb[26];
} shm;

void productor();
void consumidor();


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
    }
    else if(pid > 0){
        buffer = shmat( id_zone, (char*) 0, 0);
        if(buffer == NULL){
            fprintf(stderr, "Error reserve shared memory \n");
            exit(EXIT_FAILURE);
        }
        consumidor(buffer, semid);
        wait(NULL);

    }
    else if (pid == 0){
        id_zone = shmget(key1, sizeof(shm), 0);
        if( id_zone == -1){
            fprintf(stderr, "2 Error with id_zone \n");
            exit(EXIT_FAILURE);
        }
        buffer = shmat( id_zone, (char*) 0, 0);
        if(buffer == NULL){
            fprintf(stderr, "Error reserve shared memory \n");
            exit(EXIT_FAILURE);
        }

        if(Crear_Semaforo(key2, 2, &semid) != 1){
            fprintf(stderr, "Error 2 en Crear_Semaforo");
            return -1;
        }

        productor(buffer, semid);
        shmdt ((char *)buffer);
        exit(EXIT_SUCCESS);
    }
    shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
    Borrar_Semaforo(semid);
    return 0;
}

void productor(shm* buffer, int semid){
    int i;
    for(i=65; i<=90; i++){
        while(Down_Semaforo(semid, 1, SEM_UNDO) != OK);
        buffer->alfb[i] = (char) i;
        while(Up_Semaforo(semid, 1, SEM_UNDO) != OK);
        while(Up_Semaforo(semid, 0, SEM_UNDO) != OK);
    }
    return;
}

void consumidor(shm* buffer, int semid){
    int i;
    for(i=65; i<=90; i++){
        while(Down_Semaforo(semid, 0, SEM_UNDO) != OK);
        while(Down_Semaforo(semid, 1, SEM_UNDO) != OK);
        fprintf(stdout, "Letra %d: %c\n", i-64, buffer->alfb[i]);
        while(Up_Semaforo(semid, 1 ,SEM_UNDO != OK));
    }
    return;
}
