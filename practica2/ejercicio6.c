#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


int main(){
    int pid;

    pid=fork();

    if(pid==0){
        printf("Soy el proceso hijo con pid %d\n", getpid());
        sleep(5);
    }
    else{
        sleep(30);
        kill(pid,9);
        exit(EXIT_SUCCESS);
    }
}