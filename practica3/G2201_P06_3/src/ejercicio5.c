/**
* @brief Programa correspondiente al ejercicio 5 de la práctica 3 de Sistemas Operativos
*
* Este programa prueba que funciona nuestra libreria semaforos.h implementada en el ejercicio anterior
*
* @file ejercicio5.c
* @author Luis Carabe y Emilio Cuesta
* @date 05-04-2017
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
#include "semaforos.h"

#define FILEKEY "/bin/cat" /*Util para ftok */
#define KEY 1300
#define SEMNUM 5

/**
* @brief Estructura guardada en la region de memoria compartida.
*
* Esta estructura dispone de dos campos, una cadena de caracteres que almacena el nombre
* de un cliente y un entero que seria su id.
*/
typedef struct _info{
    char nombre[80];
    int id;
} info;

/**
* @brief Manejador definido para la senial SIGUSR1.
*
* Imprime el nombre y el id de un usuario por pantalla
*/
void manejador_SIGUSR1();


/*Variable global del tipo struct a la que tienen acceso los dos procesos.
Se declara global para que en el codigo del manejador se pueda acceder a ella*/
info* buffer;

/**
* @brief Esta funcion (del ejercicio 2) crea una zona de memoria compartida,
* posteriormente, realiza un fork. Desde el proceso hijo se solicita la informacion
* de los nuevos clientes por pantalla. Desde el proceso padre, se imprime desde el
* manejador esta informacion cuando se recibe la senial SIGUSR1 que envia el hijo
* una vez recibida la informacion. En este ejercicio lo implementamos con semáforos
* @param int num  Numero de usuarios que se van a introducir.
*/

int ejercicio2Sem(int num);

/**
* @brief Funcion main del programa que primero comprueba que todas las funciones
* de la libreria semaforos.h realizan bien su trabajo, tras esto, aplicamos
* las funciones a un ejemplo práctico, como el realizado en el ejercicio2.c, que 
* ahora implementamos con semáforos, quitando el sleep
*/

int main(){

    int semid, key;
    int i, num;
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
        free(init);
        return -1;
    }

    semctl(semid,0, GETALL, arg);
    for(i=0; i<SEMNUM; i++){
        if(arg.array[i] != i){
            fprintf(stderr, "Error en Inicializar_Semaforo (valores)\n");
            free(init);
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

    fprintf(stdout, "No se han producido errores en la primera prueba de la librería \"semáforos.h\"\n");
    free(arg.array);

    /* A continuación adaptaremos el código del ejercicio 2 para que funcione debidamente con semáforos*/
    
    fprintf(stdout, "Introduce el número de procesos hijos a generar en el ejercicio2 realizado con semáforos:\n");
    fscanf(stdin, "%d", &num);

    if(ejercicio2Sem(num)==-1){
        perror("Error en el ejercicio2 con semáforos\n");
        return -1;    
    }

    fprintf(stdout, "No se han producido errores en la segunda prueba de la librería \"semáforos.h\"\n");

    return 0;
}

int ejercicio2Sem(int num){

    int id_zone, pid, semid;
    int i;
    int key, key2;
    unsigned short* init;
    union semun {
        int val;
        struct semid_ds *semstat;
        unsigned short *array;
    } arg;

    key2 = ftok(FILEKEY, KEY);
    if (key2 == -1) {
        fprintf (stderr, "Error with key \n");
        return -1;
    }

    /*Creamos el semáforo*/

    if(Crear_Semaforo(key2, 1, &semid) != 0){
        fprintf(stderr, "Error en Crear_Semaforo(retorno)\n");
        return -1;
    }
    if(semid < 1){
        fprintf(stderr, "Error en Crear_Semaforo(id mal generado)\n");
        return -1;
    }

    /*Inicializamos el semaforo*/
    init = (unsigned short*) malloc (1*sizeof(unsigned short));
    if(init == NULL){
        return -1;
    }

    init[0] = 1;

    if(Inicializar_Semaforo(semid, init) == ERROR){
        fprintf(stderr, "Error en Inicializar_Semaforo (Retorno)\n" );
        free(init);
        return -1;
    }

    /*Armamos el manejador de la señal SIGUSR1*/

    if(signal (SIGUSR1, manejador_SIGUSR1)==SIG_ERR){
        perror("Error  definiendo el manejador de senales");
        free(init);
        exit(EXIT_FAILURE);
    }

    /*Creamos la zona de memoria compartida*/

    key = ftok(FILEKEY, KEY);
    if (key == -1) {
        fprintf (stderr, "Error with key \n");
        free(init);
        return -1;
    }

    id_zone = shmget(key, sizeof(info), IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
    if( id_zone == -1){
        fprintf(stderr, "Error with id_zone \n");
        free(init);
        exit(EXIT_FAILURE);
    }

    buffer = shmat( id_zone, (char*) 0, 0);
    if(buffer == NULL){
        fprintf(stderr, "Error reserve shared memory \n");
        free(init);
        exit(EXIT_FAILURE);
    }

    for (i=0, pid = 1; pid!=0 && i < num; i++){
        pid = fork();
        if(pid == -1){
            perror("Error realizando el fork");
        }
        if(pid == 0){
            /*Ya no tenemos que hacer sleep, simplemente utilizamos el semáforo*/
            if(Down_Semaforo(semid, 0, SEM_UNDO) == ERROR){
                fprintf(stderr, "Error en Down_Semaforo (Retorno)\n" );
                return -1;
            }
            printf("ALTA DE UN NUEVO CLIENTE (Proceso %d). Introduzca su nombre:\n",  getpid());
            scanf("%s", buffer->nombre);
            buffer->id++;
            kill(getppid(), SIGUSR1);
            shmdt ((char *)buffer);
            if(Up_Semaforo(semid, 0, SEM_UNDO) == ERROR){
                fprintf(stderr, "Error en Up_Semaforo (Retorno)\n" );
                return -1;
            }
            exit(EXIT_SUCCESS);

        }
    }
    for(i=0; i < num; i++){
        wait(NULL);
    }
    
    /*Liberamos recursos*/

    if(Borrar_Semaforo(semid) == ERROR){
        fprintf(stderr, "Error en Borrar_Semaforo");
        free(init);
        return -1;
    }
    free(init);
    shmdt ((char *)buffer);
    shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
    return 0;
}

void manejador_SIGUSR1(){
    printf("Nombre de usuario: %s, ID: %d\n", buffer->nombre, buffer->id);

    if(signal (SIGUSR1, manejador_SIGUSR1)==SIG_ERR){
        perror("Error en definiciendo el manejador de senales");
        exit(EXIT_FAILURE);
    }
}
