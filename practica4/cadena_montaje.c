#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef struct _mensaje{
    long id; /* Identificador del mensaje*/ 
    /* Informacion que se quiere transmitir*/ 
    int flag;
    char mens[4096];  
}mensaje; 

#define N 33

int main(int argc, char* argv[]){
    key_t clave;
    int msqid, idB, idC, size, i;   
    char buffer[4096];
    char letter;
    mensaje msg;

    FILE* fd = fopen("debugging.txt", "a");
    if (argc != 3){
        perror("Error en los argumentos de entrada");
        exit(EXIT_FAILURE);
    }
    clave = ftok ("/bin/ls", N); /*Misma clave que el proceso cooperante*/  
    if (clave == (key_t)-1) {
        perror("Error al obtener clave para cola mensajes \n");  
        exit(EXIT_FAILURE); 
    }
    fprintf(fd,"holita\n");
    /* Se crea la cola de mensajes y se obtiene un identificador para ella.  
    * El IPC_CREAT indica que cree la cola de mensajes si no lo está.
    * 0600 son permisos de lectura y escritura para el usuario que lance
    * los procesos. Es importante el 0 delante para que se interprete en octal.  
    */
    msqid = msgget (clave, 0600 | IPC_CREAT);
    if (msqid == -1){  
        perror ("Error al obtener identificador para cola mensajes \n");  
        exit(EXIT_FAILURE);  
    }
    
    /*Realizamos los fork correspondientes*/
    idB = fork();
    if(idB == -1){
        perror("Error con el primer fork");
        msgctl (msqid, IPC_RMID, (struct msqid_ds *)NULL);
        exit(EXIT_FAILURE);
    }
    else if(idB == 0){
        idC = fork();
        if(idC == -1){
            perror("Error con el segundo fork");
            msgctl (msqid, IPC_RMID, (struct msqid_ds *)NULL);
            exit(EXIT_FAILURE);
        }
        else if(idC == 0){
            /*Codigo de C*/
            
            FILE* f2;
            f2 = fopen(argv[2],"w");
            fclose(f2);


            f2 = fopen(argv[2],"a");
            if(f2 == NULL){
                    perror("Error abriendo el segundo archivo");
                    exit(EXIT_FAILURE);
            }

            while(msg.flag == 0){
                msgrcv(msqid, (struct msgbuf*) &msg, sizeof(mensaje)-sizeof(long), 2,0);
                fwrite(&msg.mens, 4096, 1, f2);
            }

            fclose(f2);
        }
        else{
            /*Codigo de B*/
            /*msgrcv(msqid, (struct msgbuf*) &msg, sizeof(mensaje)-sizeof(long), 1,0);
            strcpy(buffer, msg.mens);*/

            while(msg.flag == 0){

                msgrcv(msqid, (struct msgbuf*) &msg, sizeof(mensaje)-sizeof(long), 1,0);
                strcpy(buffer, msg.mens);
                size = sizeof(buffer)/sizeof(char);

                for (i=0; i<size; i++){

                    letter = msg.mens[i];

                    if ((letter > 96) && (letter < 123)){
                        letter = letter - 32;
                        msg.mens[i] = letter;
                    }
                    
                }

                msg.id = 2;
                msg.flag = 0;
                msgsnd(msqid, (struct msgbuf*) &msg, sizeof(mensaje)-sizeof(long)-sizeof(int), IPC_NOWAIT);  
            } 

            msg.id = 2;
            msg.flag = 1;
            msgsnd(msqid, (struct msgbuf*) &msg, sizeof(mensaje)-sizeof(long)-sizeof(int), IPC_NOWAIT);  

            wait(NULL);
            exit(EXIT_SUCCESS);

        }
    }
    else{

        /*Codigo de A*/ 
        /*FILE* f;*/
        fprintf(fd, "no entra en el if\n");

        FILE* f = fopen("datos.txt", "r");
        if(f == NULL){
            perror("Error al abrir el fichero");
            exit(EXIT_FAILURE);
        }
        fprintf(fd, "no entra en el if\n");

        while(fread(&buffer,4096, 1, f)){
            msg.id = 1;
            fprintf(stdout, "%s", buffer);
            strcpy(msg.mens, buffer);
            msg.flag = 0;
            msgsnd(msqid, (struct msgbuf*) &msg, sizeof(mensaje)-sizeof(long)-sizeof(int), IPC_NOWAIT);
            fprintf(fd, "una lectura\n");

        }
        fprintf(stdout, "no entra en el if\n");
        fclose(fd);

        msg.id = 1;
        msg.flag = 1;       
        msgsnd(msqid, (struct msgbuf*) &msg, sizeof(mensaje)-sizeof(long)-sizeof(int), IPC_NOWAIT);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}

