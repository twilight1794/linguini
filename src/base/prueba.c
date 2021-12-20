#include <stdio.h>
#include <stdlib.h>
#include "pila.h"
#include "diccionario.h"

int main(void){
  char* a = "Heidi";
  char* b = "Ana";
  char* c = "Caro";
  char* d = "Diana";
  int *aa = (int *) malloc(sizeof(int));
  *aa = 5;
  int *bb = (int *) malloc(sizeof(int));
  *bb = 4;
  int *cc = (int *) malloc(sizeof(int));
  *cc = 11;
  int *dd = (int *) malloc(sizeof(int));
  *dd = 9;

  Dicc *z = Dicc_Crear();
  Dicc_Destruir(&z);
/*  char* a = "Heidi";
  char* b = "Ana";
  char* c = "Caro";
  char* d = "Diana";

  Pila* p = Pila_Crear();
  Pila_Anadir(p, a);
  Pila_Anadir(p, b);
  Pila_Anadir(p, c);
  Pila_Anadir(p, d);
  for (size_t i=0; i<4;i++){
    void* v;
    Pila_Quitar(p, &v);
    printf("%s\n", (char*) v);
  }
  Pila_Destruir(&p);*/
  return 0;
}
