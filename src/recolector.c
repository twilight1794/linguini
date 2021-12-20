#include "recolector.h"

Remy_Objetos* Recolector_Crear(){
  Remy_Objetos* r = (Remy_Objetos*) malloc(sizeof(Remy_Objetos));
  if (r != NULL){
    r->vals = (Remy_Objeto*) malloc(sizeof(Remy_Objeto));
    if (r->vals == NULL){
      free(r);
      return NULL;
    } else r->elems = 0;
  }
  return r;
}

int Recolector_Destruir(Remy_Objetos** r){
  size_t i = 0;
  Remy_Objeto* = (*r)->vals;
  while (i<(*r)->elems){
    if (((*r)->vals + i) != NULL) free((*r)->vals + i);
    i++;
  }
  free(r->vals);
  free(*r);
  *r = NULL;
  return (int) RECOLECTOR_OK;
}

int Recolector_Anadir(Remy_Objetos* r, void* o){
  Remy_Objeto* ptr = realloc(r->vals, sizeof(Remy_Objeto)*(r->elems+1));
  if (ptr == NULL) return (int) RECOLECTOR_ERROR_SISTEMA;
  else {
    r->vals = ptr;
    r->elems++;
    r->vals[r->elems-1]->val = o;
    r->vals[r->elems-1]->refs = 1;
  }
  return (int) RECOLECTOR_OK;
}

int Recolector_Actualizar(Remy_Objetos* r, size_t p, int op){
  if (p<r->elems) r->vals[p]->refs =+ op;
  else return (int) RECOLECTOR_INDICE_ERRONEO;
  return (int) RECOLECTOR_OK;
}

int Recolector_Eliminar(Remy_Objetos* r, size_t p){
  if (p<r->elems){
    if (r->vals[p]){
    free(r->vals[p]->val); /* FIX: Esto debe manejarse de mejor manera, val es un objeto */
    free(r->vals[p]);
    r->vals[p] = NULL;
    r->elems--;
  } else return (int) RECOLECTOR_INDICE_ERRONEO;
  return (int) RECOLECTOR_OK;
}

int Recolector_Limpiar(Remy_Objetos* r){
  size_t i = 0;
  Remy_Objeto* = (*r)->vals;
  while (i<(*r)->elems){
    if (((*r)->vals + i)->refs == 0){
      free(((*r)->vals + i)->val); /* FIX: Esto debe manejarse de mejor manera, val es un objeto */
      free((*r)->vals + i);
    }
    i++;
  }
  return (int) RECOLECTOR_OK;
}