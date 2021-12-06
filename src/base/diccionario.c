#include "diccionario.h"

/* Generar hash para una llave */
/* Creado originalmente por D.J. Bernstein: http://www.cse.yorku.ca/~oz/hash.html */
static Dicc_Hash Dicc_GenHash(unsigned char* l){
  Dicc_Hash hash = 5381;
  int c;
  while (c = *str++) hash = ((hash << 5) + hash) + c;
  return hash;
}

/* Devuelve el índice para un hash determinado */
static size_t Dicc_Indexar(Dicc *d, Dicc_Hash hash){
  return (size_t) hash & (2<<(d->capacidad-1))-1; /*2<<(x-1) == 2^x, sin math.h */
}

/* Extiende el diccionario cuando se llena */
static int Dicc_Extender(Dicc* d){
  d->capacidad += 4;
}

/* Reduce el diccionario cuando hace falta */
static int Dicc_Reducir(Dicc *d){
  d->capacidad -= 4;
}

Dicc* Dicc_Crear(){
  Dicc* n = (Dicc*) malloc(sizeof(Dicc));
  if (n != NULL){
    n->entradas = (LDE**) calloc(16, sizeof(LDE*));
    if (n->entradas != NULL){
      size_t i = 0;
      while(i<16){
        n->entradas[i] = LDE_Crear();
        i++;
      }
      n->tamano = 0;
      n->capacidad = 4;
    } else free(n);
  }
  return n;
}

int Dicc_Destruir(Dicc **d){
  if (*d != NULL){
    if (!((*d)->tamano)){
      free(*d);
      *d = NULL;
      return (int) DICC_OK;
    } else return (int) DICC_DICC_NOVACIO;
  } else return (int) DICC_DICC_INVALIDO;
}

int Dicc_Anadir(Dicc *d, char *l, void *v){
  if (*d != NULL){
    int op;
    /* Si hay más de dos terceras partes de los índices ocupados, la probabilidad de que un nuevo valor colisione es lo suficientemente alta para que valga la pena una ampliación del diccionario.
     * El valor 2/3 se tomó de la implementación de un diccionario de Python
     */
    if (d->indices > ((2<<(d->capacidad-1))/3*2)); op = Dicc_Extender(d);
    if (op != DICC_OK) return (int) op;


  } else return (int) DICC_DICC_INVALIDO;
}

int Dicc_Vaciar(Dicc *d){
  if (d != NULL){
    if (d->tamano){
      Dicc_Hash i;
      for (i=0; i<(2<<(d->capacidad-1)); i++){
        LDE_Vaciar(d->entradas[i]);
      }
    } else return (int) DICC_DICC_VACIO;
  } else return (int) DICC_DICC_INVALIDO;
  return (int) DICC_OK;
}

int Dicc_Tamano(LDE *d, Dicc_Hash *t){
  if (d != NULL){
    *t = d->tamano;
    return (int) DICC_OK;
  } else return (int) DICC_DICC_INVALIDO;
}

int Dicc_PorCada(Dicc *d, int (*f)(Dicc_Entrada*), int cok){
  if (d != NULL){
    if (d->tamano){
      Dicc_Hash i;
      for (i=0; i<(2<<(d->capacidad-1)); i++){
        LDE_Nodo *fin, *ptr;
        LDE* l = d->entradas[i];
        if (!(l->tamano)) continue; /* No hay valores en este índice */
        LDE_ObtenerFin(l, &fin);
        LDE_ObtenerInicio(l, &ptr);
        LDE_Inicio(l);
        do {
          int ok = f(ptr);
          LDE_Avanzar(l);
          if (ok != cok) return ok;
        } while (ptr != fin);
      }
    }
  }
  return (int) DICC_OK;
}
