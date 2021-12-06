#include "pila.h"

Pila* Pila_Crear(){
  Pila* n = (Pila*) calloc(1, sizeof(Pila));
  if (n != NULL){
    n->lista = NULL;
    n->tamano = 0;
  }
  return n;
}

int Pila_Destruir(Pila **pila){
  if (*pila != NULL){
    if (!((*pila)->tamano)){
      free(*pila);
      *pila = NULL;
      return (int) PILA_OK;
    } else return (int) PILA_PILA_NOVACIA;
  } else return (int) PILA_PILA_INVALIDA;
}

int Pila_Anadir(Pila* pila, void* e){
  if (pila != NULL){
    void** ptr;
    /* No se asigna directamente a pila->lista porque en caso de fallar realloc, el valor antiguo sigue siendo válido */
    size_t t = pila->tamano + 1;
    if (pila->tamano) ptr = (void**) realloc(pila->lista, t*sizeof(void*));
    else ptr = calloc(1, sizeof(void*));

    if (ptr == NULL) return (int) PILA_ERROR_SISTEMA;
    else pila->lista = ptr;

    pila->lista[pila->tamano] = e;
    pila->tamano++;
    return (int) PILA_OK;
  } else return (int) PILA_PILA_INVALIDA;
}

int Pila_Quitar(Pila* pila, void** e){
  if (pila != NULL){
    if (pila->tamano){
      /* NOTE: Con esto, la memoria aumenta al añadir elemento pero no se reduce al quitarlos */
      if (e != NULL) *e = pila->lista[pila->tamano-1];
      pila->tamano--;
      if (!pila->tamano) free(pila->lista);
      return (int) PILA_OK;
    } else return (int) PILA_PILA_VACIA;
  } else return (int) PILA_PILA_INVALIDA;
}

int Pila_Obtener(Pila* pila, void** e){
  if (pila != NULL){
    if (pila->tamano){
      /* NOTE: Con esto, la memoria aumenta al añadir elemento pero no se reduce al quitarlos */
      if (e != NULL) *e = pila->lista[pila->tamano-1];
      else return (int) PILA_DESTINO_INVALIDO;
      return (int) PILA_OK;
    } else return (int) PILA_PILA_VACIA;
  } else return (int) PILA_PILA_INVALIDA;
}

int Pila_Tamano(Pila *pila, size_t *t){
  if (pila != NULL){
    *t = pila->tamano;
    return (int) PILA_OK;
  } else return (int) PILA_PILA_INVALIDA;
}

int Pila_Vaciar(Pila *pila, void (*f)(void*)){
  if (pila != NULL){
    if (pila->tamano){
      int r;
      void* ptr;
      do {
        r = Pila_Quitar(pila, &ptr);
        if (f != NULL) f(ptr);
      } while (r == PILA_OK);
    } else return (int) PILA_PILA_VACIA;
  } else return (int) PILA_PILA_INVALIDA;
  return (int) PILA_OK;
}
