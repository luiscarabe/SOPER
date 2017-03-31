#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#define FILEKEY "/bin/cat" /*Util para ftok */
#define KEY 1300
#define MAXBUF 10

typedef struct _info{
    char nombre[80];
    int id;
} info;


void manejador_SIGUSR1(int sig);

int main (int argc, char *argv[]) {
    
    char *buffer; /* shared buffer */
    int id_zone, pid;
    int i, num, id;
    char c;
    /* Key to shared memory */
    int key;

    
    if(argc != 2){
        perror("Error en los argumentos de entrada");
        exit(EXIT_SUCCESS);
    }
    if(signal (SIGUSR1, manejador_SIGUSR1)==SIG_ERR){
        perror("signal");
        exit(EXIT_FAILURE);
    } 
    num = atoi(argv[1]);
    
    key = ftok(FILEKEY, KEY);
    if (key == -1) {
        fprintf (stderr, "Error with key \n");
        return -1;
    }
    
    id_zone = shmget(key, sizeof(info)*MAXBUF, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
    if( id_zone == -1){
        fprintf(stderr, "Error with id_zone \n");
    }

    buffer = shmat( id_zone, (char*) 0, 0);
    if(buffer == NULL){
        fprintf(stderr, "Error reserve shared memory \n");
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
            }
            pause();
        }
        if(pid == 0){
            srand(i);
            sleep(rand());
            printf("ALTA DE UN NUEVO CLIENTE:\n");
            printf("Introduzca su nombre:");
            scanf("%s", buffer);
            id = atoi(buffer[80]);
            id++;
            buffer[81]=id;
            kill(getppid(), SIGUSR1);
            exit(EXIT_SUCCESS);
        }        
    }
    shmdt ((char *)buffer);
    shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
    return 0;
}

void manejador_SIGUSR1(int sig){
    
    printf("\nNombre de usuario: %s, ID: %d", buffer, buffer[81]);
          
}
