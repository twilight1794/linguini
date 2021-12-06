#include <stdio.h>
/*#include <stdlib.h>*/
#include "utilidades.h"
#include "expresion.h"

static void imprAlExpr(LDE* simbolos);
static void imprAsiExpr(LDE* objetos);

static void imprAlExpr(LDE* simbolos){
  LDE_Nodo *ptr;
  Expr_Elem *item;
  LDE_Inicio(simbolos);
  do {
    LDE_Obtener(simbolos, &ptr);
    LDE_Nodo_Obtener(ptr, (void **) &item);
    switch (item->tipo){
    case SIMB_NUMERO:
      printf("Numero: %s\n", (char *) item->val.Cad);
      break;
    case SIMB_IDENTIFICADOR:
      printf("Identificador: %s\n", (char *) item->val.Cad);
      break;
    case SIMB_OPERADOR:
      printf("Operador: %s\n", (char *) item->val.Cad);
      break;
    case SIMB_CADENA:
      printf("Cadena: %s\n", (char *) item->val.Cad);
      break;
    case SIMB_LOGICO:
      printf("Logico: %d\n", item->val.Bool);
      break;
    default:
      puts("??");
    }
    LDE_Avanzar(simbolos);
  } while (ptr->posterior != NULL);
}

static void imprAsiExpr(LDE* objetos){
  LDE_Nodo *ptr;
  Expr_Elem *item;
  LDE_Inicio(objetos);
  do {
    LDE_Obtener(objetos, &ptr);
    LDE_Nodo_Obtener(ptr, (void **) &item);
    switch (item->tipo){
    case CONSTANTE_ENTERA:
      printf("Entero: %d\n", (int) item->val.Ent);
      break;
    case CONSTANTE_DECIMAL:
      printf("Decimal: %f\n", (double) item->val.Dec);
      break;
    case CONSTANTE_CADENA:
      printf("Cadena: %s\n", (char *) item->val.Cad);
      break;
    case CONSTANTE_LOGICA:
      printf("Logica: %d\n", (int) item->val.Bool);
      break;
    case IDENTIFICADOR:
      printf("Identificador: %s\n", (char *) item->val.Cad);
      break;
    case IDENTIFICADOR_FUNCION:
      printf("Función: %s\n", (char *) item->val.Cad);
      break;
    case SIMB_OPERADOR:
      printf("Operador: %d\n", item->val.Subtipo);
      break;
    default:
      printf("??: %d\n", item->tipo);
    }
    LDE_Avanzar(objetos);
  } while (ptr->posterior != NULL);
}

void def(void* ptr){
  char* t = (char*) malloc(1);
  *t = 'B';
  free(*((char**) ptr));
  *((char**) ptr) = (char*) t;
}

int main(void){
  LDE* s = LDE_Crear();
  LDE* t = LDE_Crear();
  unsigned char *e = (unsigned char *) "[5%\"ppp\"+Falso]*Instagram:Seguidores(a)"; /* ☁ */
  Expr_ALexico(e, s);
  imprAlExpr(s);
  puts("----------");
  Expr_ASintactico(s, t);
  imprAsiExpr(t);

  /*  char *y = malloc(1);
  *y = 'A';
  printf("Antes: %c\n", *y);
  def(&y);
  printf("Después: %c\n", *y);*/
  return 0;
}
