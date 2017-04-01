#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/shm.h>
#include "semaforos.h"


int Inicializar_Semaforo(int semid, unsigned short *array){
    int n_sem;
    union semun { 
        int val; 
        struct semid_ds *semstat; 
        unsigned short *array; 
    } arg;

    if(semid < 0){
        return ERROR;
    }
    /*TRIPLEEEE??????????? DE hecho no sé si hace falta poner el n_sem, en el ejemplo lo pone*/
    n_sem = sizeof(array)/sizeof(array[0]); 

    arg.array = array;
    if(semctl (semid, n_sem, SETALL, arg)==-1){
        return ERROR;
    }
    return OK;
}

int Borrar_Semaforo(int semid){
    if(semctl (semid,0, IPC_RMID, 0) == -1){
        return ERROR;
    }
    return OK;
}

int Crear_Semaforo(key_t key, int size, int *semid){
    int i;
    union semun { 
        int val; 
        struct semid_ds *semstat; 
        unsigned short *array; 
    } arg; 
    if (size < 0){
        return ERROR;
    }


    /*Creamos el semáforo*/
    
    *semid = semget(key, size, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W); 
    if (*semid == -1){
        if(errno == EEXIST){
            *semid = semget(key,size,SHM_R|SHM_W);
            return 1;
        }
        return ERROR;
    }
    /*Inicializamos el semáforo*/

    arg.array = (unsigned short*) malloc (size*sizeof(unsigned short));
    for(i = 0; i < size; i++){
        arg.array[i] = 0;
    }

    if(semctl (*semid, size, SETALL, arg)==-1){
        free(arg.array);
        return ERROR;
    }
    free(arg.array);
    return 0;
}
 
int Down_Semaforo(int id, int num_sem, int undo){
    struct sembuf sem_oper;
    if(id < 0 || num_sem < 0){
        return ERROR;
    }
    sem_oper.sem_num = num_sem;
    sem_oper.sem_op = -1;
    sem_oper.sem_flg = undo;
    if(semop (id, &sem_oper, 1) == -1){
        return ERROR;
    }
    return OK;
}

int DownMultiple_Semaforo(int id,int size,int undo, int *active){
    struct sembuf sem_oper;
    int i;
    if(id < 0 || size < 0){
        return ERROR;
    }
    sem_oper.sem_op = -1;
    sem_oper.sem_flg = undo;
    for(i = 0; i < size; i++){
        sem_oper.sem_num = active[i];
        if(semop (id, &sem_oper, 1) == -1){
            return ERROR;
        }
    }
    return OK;
}

int Up_Semaforo(int id, int num_sem, int undo){
    struct sembuf sem_oper;
    if(id < 0 || num_sem < 0){
        return ERROR;
    }
    sem_oper.sem_num = num_sem;
    sem_oper.sem_op = 1;
    sem_oper.sem_flg = undo;
    if(semop (id, &sem_oper, 1) == -1){
        return ERROR;
    }
    return OK;
}

int UpMultiple_Semaforo(int id,int size, int undo, int *active){
    struct sembuf sem_oper;
    int i;
    if(id < 0 || size < 0){
        return ERROR;
    }
    sem_oper.sem_op = 1;
    sem_oper.sem_flg = undo;
    for(i = 0; i < size; i++){
        sem_oper.sem_num = active[i];
        if(semop (id, &sem_oper, 1) == -1){
            return ERROR;
        }
    }
    return OK;
}
