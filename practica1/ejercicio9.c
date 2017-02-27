#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


int main (int argc, char* argv[]){
    int pid=1;
    int op1,op2;
    int res;
    int pipe1[2], pipe2[2], pipe3[2], pipe4[2];
    int pipe5[2], pipe6[2], pipe7[2], pipe8[2];
    int i, flag=0;
    char buffer[10], ret[200];


    if(pipe(pipe1)==-1||pipe(pipe2)==-1||pipe(pipe3)==-1||pipe(pipe4)==-1||
       pipe(pipe5)==-1||pipe(pipe6)==-1||pipe(pipe7)==-1||pipe(pipe8)==-1){
        perror("Error creando la tubería\n");
        exit(EXIT_FAILURE);
    }

    for(i=0; i<4 && pid!=0; i++){

        if((pid = fork()) == -1){
            perror("fork");
            exit(EXIT_FAILURE);
        }

        else if(pid==0){

            switch(i){
                case 0:
                    close(pipe1[1]); /*0 leer, 1 escribir*/
                    close(pipe2[0]);/*en este se returnea res*/
                
                    read(pipe1[0], buffer, 100*sizeof(char));
                    sscanf(buffer, "%d,%d", &op1, &op2);
                    res = op1 + op2;
                    sprintf(ret,"Datos enviados a traves de la tuberia por el proceso PID=%d. Operando1: %d. Operando2: %d. Suma:%d", getpid(), op1,op2,res);
                    write(pipe2[1],ret, sizeof(ret));
                    exit(EXIT_SUCCESS);
                    break;
                case 1:
                    close(pipe3[1]); /*0 leer, 1 escribir*/
                    close(pipe4[0]);/*en este se returnea res*/


                    read(pipe3[0], buffer, 100*sizeof(char));
                    sscanf(buffer, "%d,%d", &op1, &op2);

                    res = op1 - op2;
                    sprintf(ret,"Datos enviados a traves de la tuberia por el proceso PID=%d. Operando1: %d. Operando2: %d. Resta:%d", getpid(), op1,op2,res);
                    write(pipe4[1],ret, sizeof(ret));
                    exit(EXIT_SUCCESS);
                    break;
                case 2:
                    close(pipe5[1]); /*0 leer, 1 escribir*/
                    close(pipe6[0]); /*en este se returnea res*/

                    read(pipe5[0], buffer, 100*sizeof(char));
                    sscanf(buffer, "%d,%d", &op1, &op2);

                    res = op1 * op2;
                    sprintf(ret,"Datos enviados a traves de la tuberia por el proceso PID=%d. Operando1: %d. Operando2: %d. Multiplicacion:%d", getpid(), op1,op2,res);
                    write(pipe6[1],ret, sizeof(ret));
                    exit(EXIT_SUCCESS);
                    break;
                default:
                    close(pipe7[1]); /*0 leer, 1 escribir*/
                    close(pipe8[0]); /*en este se returnea res*/

                    read(pipe7[0], buffer, 100*sizeof(char));
                    sscanf(buffer, "%d,%d", &op1, &op2);
                    res = op1 / op2;
                    sprintf(ret,"Datos enviados a traves de la tuberia por el proceso PID=%d. Operando1: %d. Operando2: %d. Division:%d", getpid(), op1,op2,res);
                    write(pipe8[1],ret, sizeof(ret));
                    exit(EXIT_SUCCESS);
                    break;
            }
        }
    }

    close(pipe1[0]);
    close(pipe2[1]);
    close(pipe3[0]);
    close(pipe4[1]);
    close(pipe5[0]);
    close(pipe6[1]);
    close(pipe7[0]);
    close(pipe8[1]);

    fprintf(stdout,"Introduce el primer operando:\n");
    fscanf(stdin,"%d",&op1);
    fprintf(stdout,"Introduce el segundo operando:\n");
    fscanf(stdin, "%d",&op2);
    sprintf(buffer,"%d,%d",op1,op2);

    write(pipe1[1],buffer,sizeof(buffer));
    write(pipe3[1],buffer,sizeof(buffer));
    write(pipe5[1],buffer,sizeof(buffer));
    write(pipe7[1],buffer,sizeof(buffer));

    for(i=0;i<4;i++){
        wait(NULL);
    }



    read(pipe2[0],ret,sizeof(ret));
    fprintf(stdout,"%s\n", ret);
    read(pipe4[0],ret,sizeof(ret));
    fprintf(stdout,"%s\n", ret);
    read(pipe6[0],ret,sizeof(ret));
    fprintf(stdout,"%s\n", ret);
    read(pipe8[0],ret,sizeof(ret));
    fprintf(stdout,"%s\n", ret);

    exit(EXIT_SUCCESS);
}
