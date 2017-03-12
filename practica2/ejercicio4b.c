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
    int row;
    int id;
    struct _args * nextthread;
} args;

int crear_matriz(int dim, args* operacion){
    int i;

    if(dim < 0|| dim > 4 || operacion == NULL){
        return -1;
    }

    operacion->matriz = (int**)malloc(dim*sizeof(int*));
    if(!operacion->matriz){
        return -1;
    }

    for(i=0; i<dim; i++){
        operacion->matriz[i]=(int*)malloc(dim*sizeof(int));
        if(!operacion->matriz[i]){
        return -1;
        }
    }
   
    switch(dim){
        case 1:
            break;
            scanf("%d", &operacion->matriz[0][0]);
        case 2:
            scanf("%d %d %d %d", &operacion->matriz[0][0], &operacion->matriz[0][1], 
                                 &operacion->matriz[1][0], &operacion->matriz[1][1]);
            break;
        case 3:
            scanf("%d %d %d %d %d %d %d %d %d", &operacion->matriz[0][0], &operacion->matriz[0][1], &operacion->matriz[0][2], 
                                                &operacion->matriz[1][0], &operacion->matriz[1][1], &operacion->matriz[1][2], 
                                                &operacion->matriz[2][0], &operacion->matriz[2][1], &operacion->matriz[2][2]);
            break;
        default:
            scanf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &operacion->matriz[0][0], &operacion->matriz[0][1], &operacion->matriz[0][2], &operacion->matriz[0][3], 
                                                                     &operacion->matriz[1][0], &operacion->matriz[1][1], &operacion->matriz[1][2], &operacion->matriz[1][3], 
                                                                     &operacion->matriz[2][0], &operacion->matriz[2][1], &operacion->matriz[2][2], &operacion->matriz[2][3], 
                                                                     &operacion->matriz[3][0], &operacion->matriz[3][1], &operacion->matriz[3][2], &operacion->matriz[3][3]);
            break;
    }

    return 0;
}
 
void* threadbehaviour(void* n){
    int i, j;
    int resultado;    
    args* datos = (args*) n;    
    datos->row=0;

    for(i=0;i<datos->dim;i++){
        
        printf("Hilo %d multiplicando fila %d resultado ", datos->id,i );
        
        for(j=0;j<datos->dim;j++){
            
            resultado = (datos->matriz[i][j])*datos->mult; 
            printf(" %d", resultado);

        }
        if((datos->nextthread)->row < datos->dim){
            printf(" - el Hilo %d va por fila %d\n", (datos->nextthread)->id, (datos->nextthread)->row);
            datos->row++;
        }
        else{
            printf(" - el Hilo %d ya ha finalizado\n", (datos->nextthread)->id);
        }
        usleep(1000000);
    }
        
 
    pthread_exit(NULL);
}
int main(int argc, char* argv[]){
    int dim;
    pthread_t hilo[2];
    args* operacion1;
    args* operacion2;
    int i;

    do{
        printf("Introduzca dimension de la matriz cuadrada:\n");
        scanf("%d", &dim);

        if (dim > 4){
            printf("La dimension no debe exceder 4.\n");
        }
    } while(dim > 4);

    operacion1=(args*)malloc(sizeof(args));
    if(operacion1 == NULL){
        exit(EXIT_FAILURE);
    }
    operacion1->dim = dim;
    operacion2=(args*)malloc(sizeof(args));
    if(operacion2 == NULL){
        exit(EXIT_FAILURE);
    }
    operacion2->dim = dim;

    printf("Introduzca multiplicador 1:\n");
    scanf("%d", &operacion1->mult);
    operacion1->id = 1;

    printf("Introduzca multiplicador 2:\n");
    scanf("%d", &operacion2->mult);
    operacion2->id = 2;

    printf("Introduzca matriz 1:\n");
    crear_matriz(dim, operacion1);
    
    printf("Introduzca matriz 2:\n");
    crear_matriz(dim, operacion2);

    operacion1->nextthread=operacion2;
    operacion2->nextthread=operacion1;

    printf("Realizando producto:\n");
    pthread_create(&hilo[0], NULL, threadbehaviour, (void*) operacion1);
    pthread_create(&hilo[1], NULL, threadbehaviour, (void*) operacion2);

    pthread_join(hilo[0],NULL);
    pthread_join(hilo[1],NULL);

    for(i=0; i<dim; i++){
        free(operacion1->matriz[i]);
        free(operacion2->matriz[i]);
    }

    free(operacion1->matriz);
    free(operacion2->matriz);
    free(operacion1);
    free(operacion2);

    printf("El programa %s termino correctamente. \n", argv[0]);
    exit(EXIT_SUCCESS);
}
