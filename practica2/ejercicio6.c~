#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


int main(){
    int pid;

    pid=fork();

    if(pid==0){
        pid = getpid();
        while(1){
            printf("Soy el proceso hijo con pid %d\n", pid);
            sleep(5);
        }
    }
    else{
        sleep(30);
        if(kill(pid,9) == -1){
            perror("Se ha producido un error enviando la señal");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
}