#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

typedef enum {FALSE=0, TRUE=1} Bool;



Bool es_primo(int p){
    int i;
    int s;
    s = sqrt(p);
    for (i = 2; i <= s; i++){
        if ((p % i) == 0){
            return FALSE;
        }
    }
    return TRUE;
} 

void* threadbehaviour(void* n){
    int counter;
    int i, num;

    num = *((int*)n); 
    counter = 0;

    for(i=1; counter!=num; i++){

        if(es_primo(i)){
            counter++;
        }
    }

    pthread_exit(NULL);

}

int main(int argc, char* argv[]){

    int n;
    int i;
    struct timeval time1, time2;
    double time;
    pthread_t ids[100];

    if (argc != 2){
        perror("Es necesario pasar como argumento el numero de primos a calcular\n");
        exit(EXIT_FAILURE);
    }
    
    n = atoi(argv[1]);

    gettimeofday(&time1,NULL);

    for(i=0; i < 100; i++){
        pthread_create(&ids[i], NULL, threadbehaviour, (void*) &n);
    }

    for(i=0; i < 100; i++){
        pthread_join(ids[i], NULL);
    }

    gettimeofday(&time2, NULL);
    time = (time2.tv_sec - time1.tv_sec) + (time2.tv_usec - time1.tv_usec)*1e-6;

    printf("El programa %s termino correctamente, con un tiempo de %f segundos.\n", argv[0], time);
    exit(EXIT_SUCCESS);
}