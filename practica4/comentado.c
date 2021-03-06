/**
* @brief Programa correspondiente al ejercicio 1 de la práctica 3 de Sistemas Operativos
*
* Este programa crea tres procesos: el principal, proceso A, que se encarga de hacer un fork para crear
* el proceso B, y este, a su vez, ejecuta un fork para crear el proceso C. El proceso A leerá de un fichero
* pasado como argumento cadenas de caracteres de 4096 en 4096, que enviará al proceso B mediante una cola de
* mensajes, este cambiará todas las letras a mayúsculas y enviará el mensaje transformado al proceso C mediante 
* otro mensaje, para que lo imprime en el segundo fichero pasado como argumento.
*
* @file cadena_montaje.c
* @author Luis Carabe y Emilio Cuesta
* @date 12-05-2017
*/

#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#define N 33

/**
* @brief Estructura que define un mensaje.
*
* Tiene cuatro campos, un identificador de mensaje, un flag que nos sirve
* para saber si se trata de la última lectura , un nbytes que nos da el tamaño
* del último mensaje y por último el array de chars que forma el mensaje a envíar
*/

typedef struct _mensaje{
    long id; /* Identificador del mensaje*/ 
    /* Informacion que se quiere transmitir*/ 
    int flag;
    int nbytes;
    char mens[4097];  
}mensaje; 

/**
* @brief Funcion main del programa que realiza la funcionalidad descrita anteriormente.
* 
* @param argv[] el primer parámetro es el fichero para leer y el segundo el fichero en el que escribir
*/

int main(int argc, char* argv[]){
    key_t clave;
    int msqid, idB, idC, size, i, aux;   
    char buffer[4096];
    char letter;
    mensaje msg;
    
    if (argc != 3){
        perror("Error en los argumentos de entrada");
        exit(EXIT_FAILURE);
    }
    clave = ftok ("/bin/ls", N); /*Misma clave que el proceso cooperante*/  
    if (clave == (key_t)-1) {
        perror("Error al obtener clave para cola mensajes \n");  
        exit(EXIT_FAILURE); 
    }
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
                msgrcv(msqid, (struct msgbuf*) &msg, sizeof(mensaje)-sizeof(long)-sizeof(int), 2,0);
                if(msg.flag == 1){
                    fwrite(&msg.mens, 1, msg.nbytes-1, f2);
                    fclose(f2);
                    exit(EXIT_SUCCESS);
                }
                fwrite(&msg.mens, 1, strlen(msg.mens)*sizeof(char), f2);
            }

        }
        else{
            /*Codigo de B*/

            while(msg.flag == 0){

                msgrcv(msqid, (struct msgbuf*) &msg, sizeof(mensaje)-sizeof(long), 1,0);

                size = strlen(msg.mens);
                for (i=0; i<size; i++){

                    letter = msg.mens[i];

                    if ((letter > 96) && (letter < 123)){
                        letter = letter - 32;
                        msg.mens[i] = letter;
                    }
                    
                }

                msg.id = 2;
                if(msg.flag == 1){
                    msg.flag = 1;
                    msgsnd(msqid, (struct msgbuf*) &msg, sizeof(msg)-sizeof(long)-sizeof(int), IPC_NOWAIT);  
                    wait(NULL);
                    exit(EXIT_SUCCESS);
                }

                msg.flag = 0;
                msgsnd(msqid, (struct msgbuf*) &msg, sizeof(msg)-sizeof(long)-sizeof(int), IPC_NOWAIT);  
            } 

        }
    }
    else{

        /*Codigo de A*/ 
        /*FILE* f;*/

        FILE* f = fopen(argv[1], "r");
        if(f == NULL){
            perror("Error al abrir el fichero");
            exit(EXIT_FAILURE);
        }

        
        aux = fread(&buffer,1, 4096, f);
        while(aux == 4096){
            msg.id = 1;
            fprintf(stdout, "%s", buffer);
            strcpy(msg.mens, buffer);
            msg.flag = 0;
            msgsnd(msqid, (struct msgbuf*) &msg, sizeof(mensaje)-sizeof(long), IPC_NOWAIT);
            aux = fread(&buffer,1, 4096, f);
        }        

        fprintf(stdout, "\n\nSEGUNDA SECCION\n\n");
        strcpy(msg.mens, buffer);
        msg.mens[aux] = '\0';

        fprintf(stdout, "%s", msg.mens);
     
        fprintf(stdout, "\nFIN DEL FICHERO\n");

        msg.id = 1;
        msg.flag = 1;
        msg.nbytes = aux;
        msgsnd(msqid, (struct msgbuf*) &msg, sizeof(mensaje)-sizeof(long), IPC_NOWAIT);

        wait(NULL);
        fclose(f);
        exit(EXIT_SUCCESS);
    }
}