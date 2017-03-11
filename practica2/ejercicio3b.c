#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>


int main(int argc, char* argv()){

    int n;

    if (argc != 2){
        perror("Es necesario pasar como argumento el numero de primos a calcular");
        exit(EXIT_FAILURE);
    }
    
    n = atoi(argv[0]);


    exit(EXIT_SUCCESS);
}