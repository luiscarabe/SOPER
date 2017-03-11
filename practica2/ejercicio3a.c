#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

typedef enum _Bool {FALSE=0, TRUE=1} Bool;

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

int main(int argc , char *argv[]) {
    
    if (argc != 2){
        printf("Error en los argumentos de entrada");
        exit(EXIT_FAILURE);
    }

    printf("El programa %s termino correctamente \n", argv[0]);
    exit(EXIT_SUCCESS);
}
