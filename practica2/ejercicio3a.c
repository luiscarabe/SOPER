#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>

typedef enum {FALSE=0, TRUE=1} Bool;

Bool es_primo(int p){
    int i;
    double s;
    s = sqrt(p);
    for (i = 2; i <= s; i++){
        if ((p % i) == 0){
            return FALSE;
        }
    }
    return TRUE;
} 

int main(int argc , char *argv[]) {
    int pid;
    int i, counter;
    int n;
    struct timeval time1, time2;
    double time;

    if (argc != 2){
        printf("Error en los argumentos de entrada.\n");
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);

    gettimeofday(&time1, NULL);

    for(i=0; i < 100; i++){
        pid = fork();
        if(pid == -1){
            printf("Error en el fork");
            exit(EXIT_FAILURE);
        }
        else if(pid == 0){
            for(counter = 0, i = 2; counter != n; i++){
                if (es_primo(i)){
                    counter++;
                }
            }
            exit(EXIT_SUCCESS);
        }
    }

    for(i=0; i < 100; i++){
        wait(NULL);
    }

    gettimeofday(&time2, NULL);

    time = ((time2.tv_sec*1e6 + time2.tv_usec) - (time1.tv_sec*1e6 + time1.tv_usec))*1e-6;



    printf("El programa %s termino correctamente, con un tiempo de %lf segundos.\n", argv[0], time);
    exit(EXIT_SUCCESS);
}
