#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LENGHT 80

int main (){
  char* cadena;
  int pid;

  cadena=(char*)malloc(LENGHT*sizeof(char));

  pid=fork();
  if(pid<0){
    printf("Error haciendo fork\n");
    exit(EXIT_FAILURE);
  }
  else if (pid == 0){
    printf("Introduzca un nombre, por favor:\n");
    scanf("%s",cadena);
    fprintf(stdout,"El nombre que que el proceso hijo ha leido es %s\n", cadena);
    exit(EXIT_SUCCESS);
  }
  else{
    /*El proceso padre espera a que acabe el hijo, asi nos aseguramos de que ya se ha insertado el nombre*/
    wait(NULL);
  }
  fprintf(stdout,"El nombre que que el proceso padre lee es %s\n", cadena);
  free(cadena);
  exit(EXIT_SUCCESS);
}

/*No podemos ver el resultado correctamente desde el proceso padre, no se por que, yo pensaba que si*/
