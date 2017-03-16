/**
* @brief Programa correspondiente al ejercicio 10 de la practica 2 de sistemas operativos
*
* Este programa se utilizara para estudiar la coordinacion entre dos procesos a la hora de escribir
* y leer un fichero de texto.
* 
* @file ejercicio10.c
* @author Luis Carabe y Emilio Cuesta
* @date 17-03-2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <time.h> 

#define NCADENAS 50
#define TAMANIO 13
#define FILEPATH "ficheroe10.txt"

/**
 * @brief Manejador para la señal SIGUSR1. No tiene ninguna funcionalidad, tan solo se utiliza
 * para que el padre pueda procesar las señales de este tipo que le manda su hijo
 * @param sig Señal a la que se aplica este manejador, previamente vinculada utilizando signal()
 * @return Sin retorno.
 */
void manejador_SIGUSR1(int sig);

/**
 * @brief Funcion main del programa. Genera dos procesos (padre e hijo), el hijo escribirá en el fichero
 * "ficheroe10.txt" palabra a palabra (escogidas aleatoriamente de las de la frase propuesta) y el padre leera una de ellas cada 5 segundos. En el momento en 
 * que el hijo imprima la palabra "FIN", acabará. Cuando el padre lea este "FIN", entenderá que el hijo
 * ya ha acabado y creara otro que realice la misma funcion que el anterior. Este proceso se repite una
 * y otra vez hasta que el proceso padre haya leido 50 palabras. 
 * @return EXIT_SUCCESS (todo ha ido bien) o EXIT FAILURE (algo ha fallado).
 */
int main(){

    int i, counter;
    int index;
    int flag;
    int pid;
    char* frase[]={"EL","PROCESO","A", "ESCRIBE", "EN", "UN", "FICHERO", "HASTA", "QUE", "LEE", "LA", "CADENA", "FIN"};
    char  aux[30];
    FILE* f;
    
    /*Se define el manejador que se activara al recibir la señal SIGUSR1"*/
    if(signal(SIGUSR1,manejador_SIGUSR1) == SIG_ERR){
        perror("Error en la captura.\n");
        exit(EXIT_FAILURE);
    }

    i = 0;
    counter = 0;

    while(1){

        /*Estas dos instrucciones se utilizan para vaciar el fichero*/
        f=fopen(FILEPATH, "w");
        fclose(f);

        printf("###### FRASE ALEATORIA %d ######\n", i+1);
        flag=0;
        pid=fork();

        if(pid == -1){

            perror("Error de fork");
            exit(EXIT_FAILURE);

        }else if(pid == 0){

            /*Semilla para que la generacion de numeros se asemeje mas a algo aleatorio*/  
            srand(getpid());

            while(1){

                f = fopen(FILEPATH, "w");
                index = rand()%TAMANIO;
                fprintf(f,"%s ",frase[index]);
                printf("El hijo ha escrito \"%s\"\n", frase[index]);                
                
                /*El hijo envia una señal cuando ya ha escrito una palabra de cada bloque, para que 
                el padre no lea de un fichero vacio*/
                if(flag == 0){

                    if(kill(getppid(),SIGUSR1) == -1){
                        perror("Error enviando senial");
                        exit(EXIT_FAILURE);
                    }

                    flag = 1;
                }

                if(strcmp(frase[index], "FIN") == 0){
                    fclose(f);
                    exit(EXIT_SUCCESS);
                }

                fclose(f);
            }

        }else{
           
            pause();

            do{

            f=fopen(FILEPATH, "r");
            fscanf(f,"%s",aux);

            if(strcmp(aux,"")){
                counter++;
                printf("El padre ha leido \"%s\", es la palabra %d\n", aux, counter);
            }

            fclose(f); 

            if(counter == NCADENAS){

                if(kill(pid,15)==-1){
                    perror("Error enviando senial");
                    exit(EXIT_FAILURE);
                }

                wait(NULL);
                exit(EXIT_SUCCESS);
            }

            sleep(5);

           } while(strcmp(aux,"FIN"));

        }

        i++;
    }

}

void manejador_SIGUSR1(int sig){

}