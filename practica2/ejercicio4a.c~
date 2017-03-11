#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

typedef struct _args{
    int dim;
    int** matriz;
    int mult;
    int id;
} args;


 
void* threadbehaviour(void* n){
    int i, j;
    int resultado;    
    args* datos = (args*) n;    
   

    for(i=0;i<datos->dim;i++){
        printf("Hilo %d multiplicando fila %d resultado", datos->id,i);
        for(j=0;j<datos->dim;j++){
            resultado=datos->matriz[i][j]*datos->mult; 
            printf(" %d", resultado);                   
        }
        printf("\n");
        usleep(1000000);
    }
        
 
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int dim;
    char nums[100], nums2[100];
    char aux;
    pthread_t hilo[2];
    args* operacion1;
    args* operacion2;
    int i;

    do{
        printf("Introduzca dimension de la matriz cuadrada:\n");      
        fscanf(stdin, "%d", &dim);

        if (dim > 4){
            printf("La dimension no debe exceder 4.\n");
        }
    } while(dim > 4);
    
    operacion1=(args*)malloc(sizeof(args));
    operacion1->dim = dim;
    operacion2=(args*)malloc(sizeof(args));
    operacion2->dim = dim;

    printf("Introduzca multiplicador 1:\n");
    scanf("%d", &operacion1->mult);
    operacion1->id = 1;

    printf("Introduzca multiplicador 2:\n");
    scanf("%d", &operacion2->mult);
    operacion2->id = 2;
    
    
    operacion1->matriz=(int**)malloc(dim*sizeof(int*));
    for(i=0;i<dim;i++){
        operacion1->matriz[i]=(int*)malloc(dim*sizeof(int));
    }

    operacion2->matriz=(int**)malloc(dim*sizeof(int*));
    for(i=0;i<dim;i++){
        operacion2->matriz[i]=(int*)malloc(dim*sizeof(int));
    }

    printf("Introduzca matriz 1:\n");
    fgets(nums, 10, stdin);
    scanf("%[^\n]s", nums);
    switch(dim){
        case 1:
            sscanf(nums,"%d", &operacion1->matriz[0][0]);
            break;
        case 2:
            sscanf(nums,"%d %d %d %d", &operacion1->matriz[0][0], &operacion1->matriz[0][1], &operacion1->matriz[1][0], &operacion1->matriz[1][1]);
            break;
        case 3:

            sscanf(nums,"%d %d %d %d %d %d %d %d %d", &operacion1->matriz[0][0], &operacion1->matriz[0][1], &operacion1->matriz[0][2], &operacion1->matriz[1][0], &operacion1->matriz[1][1], &operacion1->matriz[1][2], &operacion1->matriz[2][0], &operacion1->matriz[2][1], &operacion1->matriz[2][2]);
            break;
        case 4:
            sscanf(nums,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &operacion1->matriz[0][0], &operacion1->matriz[0][1], &operacion1->matriz[0][2], &operacion1->matriz[0][3], &operacion1->matriz[1][0], &operacion1->matriz[1][1], &operacion1->matriz[1][2], &operacion1->matriz[1][3], &operacion1->matriz[2][0], &operacion1->matriz[2][1], &operacion1->matriz[2][2], &operacion1->matriz[2][3], &operacion1->matriz[3][0], &operacion1->matriz[3][1], &operacion1->matriz[3][2], &operacion1->matriz[3][3]);
            break;
    }
    
    printf("Introduzca matriz 2:\n");
    fgets(nums, 10, stdin);
    scanf("%[^\n]s", nums);
    switch(dim){
        case 1:
            sscanf(nums,"%d", &operacion2->matriz[0][0]);
            break;
        case 2:
            sscanf(nums,"%d %d %d %d", &operacion2->matriz[0][0], &operacion2->matriz[0][1], &operacion2->matriz[1][0], &operacion2->matriz[1][1]);
            break;
        case 3:

            sscanf(nums,"%d %d %d %d %d %d %d %d %d", &operacion2->matriz[0][0], &operacion2->matriz[0][1], &operacion2->matriz[0][2], &operacion2->matriz[1][0], &operacion2->matriz[1][1], &operacion2->matriz[1][2], &operacion2->matriz[2][0], &operacion2->matriz[2][1], &operacion2->matriz[2][2]);
            break;
        case 4:
            sscanf(nums,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &operacion2->matriz[0][0], &operacion2->matriz[0][1], &operacion2->matriz[0][2], &operacion2->matriz[0][3], &operacion2->matriz[1][0], &operacion2->matriz[1][1], &operacion2->matriz[1][2], &operacion2->matriz[1][3], &operacion2->matriz[2][0], &operacion2->matriz[2][1], &operacion2->matriz[2][2], &operacion2->matriz[2][3], &operacion2->matriz[3][0], &operacion2->matriz[3][1], &operacion2->matriz[3][2], &operacion2->matriz[3][3]);
            break;
    }

    printf("Realizando producto:\n");
    pthread_create(&hilo[0], NULL, threadbehaviour, (void*) operacion1);
    pthread_create(&hilo[1], NULL, threadbehaviour, (void*) operacion2);

    pthread_join(hilo[0],NULL);
    pthread_join(hilo[1],NULL);

    printf("El programa %s termino correctamente. \n", argv[0]);
    exit(EXIT_SUCCESS);
}    
