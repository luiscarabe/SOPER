/**
* @brief Programa correspondiente al ejercicio 2 de la práctica 3 de Sistemas Operativos
*
* Este programa consiste en crear una zona de memoria que dos procesos distintos puedan compartir.
* En este caso, uno de los procesos se encargará de obtener el nombre de una serie de clientes de
* pantalla y generar un id distinto para cada uno de ellos mientras que el otro tendra que imprimir
* tanto el nombre como el id por pantalla. Para compartir esta informacion se utiliza una estructura
* en la region de memoria compartida.
*
* @file ejercicio2.c
* @author Luis Carabe y Emilio Cuesta
* @date 04-04-2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define FILEKEY "/bin/cat" /*Util para ftok */
#define KEY 1300


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
* @brief Funcion main del programa. Esta funcion crea una zona de memoria compartida,
* posteriormente, realiza un fork. Desde el proceso hijo se solicita la informacion
* de los nuevos clientes por pantalla. Desde el proceso padre, se imprime desde el
* manejador esta informacion cuando se recibe la senial SIGUSR1 que envia el hijo
* una vez recibida la informacion.
* @param int n  Numero de usuarios que se van a introducir.
*/
int main (int argc, char *argv[]) {


    int id_zone, pid;
    int i, num;
    /* Key to shared memory */
    int key;


    if(argc != 2){
        perror("Error en los argumentos de entrada");
        exit(EXIT_SUCCESS);
    }

    if(signal (SIGUSR1, manejador_SIGUSR1)==SIG_ERR){
        perror("Error  definiendo el manejador de senales");
        exit(EXIT_FAILURE);
    }

    num = atoi(argv[1]);

    key = ftok(FILEKEY, KEY);
    if (key == -1) {
        fprintf (stderr, "Error with key \n");
        return -1;
    }

    id_zone = shmget(key, sizeof(info), IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
    if( id_zone == -1){
        fprintf(stderr, "Error with id_zone \n");
        exit(EXIT_FAILURE);
    }

    for (i=0, pid = 1; pid!=0 && i < num; i++){
        pid = fork();
        if(pid == -1){
            perror("Error realizando el fork");
        }
        if(pid > 0){
            buffer = shmat( id_zone, (char*) 0, 0);
            if(buffer == NULL){
                fprintf(stderr, "Error reserve shared memory \n");
                exit(EXIT_FAILURE);
            }
        }
        if(pid == 0){
            id_zone = shmget(key, sizeof(info), 0);
            if( id_zone == -1){
                fprintf(stderr, "Error with id_zone \n");
                exit(EXIT_FAILURE);
            }
            buffer = shmat( id_zone, (char*) 0, 0);
            if(buffer == NULL){
                fprintf(stderr, "Error reserve shared memory \n");
                exit(EXIT_FAILURE);
            }
            srand(getpid());
            sleep(rand()%20);
            printf("ALTA DE UN NUEVO CLIENTE (Proceso %d). Introduzca su nombre:\n", getpid());
            scanf("%s", buffer->nombre);
            buffer->id++;
            kill(getppid(), SIGUSR1);
            shmdt ((char *)buffer);
            exit(EXIT_SUCCESS);
        }
    }
    for(i=0; i < num; i++){
        wait(NULL);
    }
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
