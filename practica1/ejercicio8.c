#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


int main (int argc, char** argv){
    int pid = 1;
    int i;
    char *prog[2];
    char *aux;

    if (argc < 2){
        printf("Error en los argumentos de entrada");
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < (argc-2) && pid > 0; i++){

        pid = fork();

        if(pid < 0){
            printf("Error en el fork.\n");
        }

        else if(pid == 0){
            prog[0] = argv[i+1];
            prog[1] = NULL;
            /*Reservamos memoria para aux, donde guardaremos el path de los programas*/
            aux = (char*)malloc(4*strlen(prog[0])*sizeof(char));

            if(aux == NULL){
                perror("Error al resevar memoria.\n");
                exit(EXIT_FAILURE);
            }

            sprintf(aux, "/bin/%s", prog[0]);


            if((strcmp(argv[argc-1], "-l")) == 0){
                execl(aux, prog[0], NULL);
                perror("Error en exec");
                free(aux);
                exit(EXIT_FAILURE);
            }

            else if((strcmp(argv[argc-1], "-lp")) == 0){
                execlp(prog[0], prog[0], NULL);
                perror("Error en exec");
                free(aux);
                exit(EXIT_FAILURE);
            }
            else if((strcmp(argv[argc-1], "-v")) == 0){
                execv(aux, prog);
                perror("Error en exec");
                free(aux);
                exit(EXIT_FAILURE);
            }
            else if((strcmp(argv[argc-1], "-vp")) == 0){
                execvp(prog[0], prog);
                perror("Error en exec");
                free(aux);
                exit(EXIT_FAILURE);
            }
            else {
                printf("Error en el último parámetro.\n");
            }
            free(aux);
        }

    }

    for(i=0; i < (argc -2); i++){
        wait(NULL);
    }
    exit(EXIT_SUCCESS);

}
