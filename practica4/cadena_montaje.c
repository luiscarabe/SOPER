#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>  
#include <stdio.h>

typedef struct _mensaje{
    long id; /* Identificador del mensaje*/ 
    /* Informacion que se quiere transmitir*/ 
    char mens[4*(2^10)];  
}mensaje; 

int main(int argc, char* argv[]){
    key_t clave;
    int msqid, idB, idC;
    int aux = 1;
    char* buffer[(4*2^10)]
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
        
        }
        
    }

}
