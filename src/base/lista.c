#include "lista.h"

LDE_Nodo* LDE_Nodo_Crear(void *x){
  LDE_Nodo* n = (LDE_Nodo*) malloc(sizeof(LDE_Nodo));
  if (n != NULL){
    n->dato = x;
    n->anterior = NULL;
    n->posterior = NULL;
  }
  return n;
}

int LDE_Nodo_Obtener(LDE_Nodo *n, void** v){
  if (n != NULL){
    *v = n->dato;
    return (int) LDE_OK;
  } else return (int) LDE_NODO_INVALIDO;
}

int LDE_Nodo_Poner(LDE_Nodo *n, void* v){
  if (n != NULL){
    n->dato = v;
    return (int) LDE_OK;
  } else return (int) LDE_NODO_INVALIDO;
}


LDE* LDE_Crear(bool circular){
  LDE* n = (LDE*) malloc(sizeof(LDE));
  if (n != NULL){
    n->inicio = NULL;
    n->fin = NULL;
    n->cursor = NULL;
    n->tamano = 0;
    n->circular = circular;
  }
  return n;
}

int LDE_Destruir(LDE **lista){
  if (*lista != NULL){
    if (!((*lista)->tamano)){
      free(*lista);
      *lista = NULL;
      return (int) LDE_OK;
    } else return (int) LDE_LISTA_NOVACIA;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_AnadirAnterior(LDE *lista, void *v){
  LDE_Nodo *n = LDE_Nodo_Crear(v);
  if (lista != NULL){
    if (lista->tamano){
      LDE_Nodo *ptr, *inicio;
      LDE_Obtener(lista, &ptr);
      LDE_Inicio(lista);
      LDE_Obtener(lista, &inicio);
      LDE_Apuntar(lista, ptr);
      if (ptr != inicio){
        ptr->anterior->posterior = n;
        n->anterior = ptr->anterior;
      } else {
        lista->inicio = n;
        if (lista->circular){
          lista->fin->posterior = n;
          n->anterior = lista->fin;
        }
      }
      ptr->anterior = n;
      n->posterior = ptr;
    } else lista->inicio = lista->fin = lista->cursor = n;
    lista->tamano++;
    return (int) LDE_OK;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_AnadirPosterior(LDE *lista, void *v){
  LDE_Nodo *n = LDE_Nodo_Crear(v);
  if (lista != NULL){
    if (lista->tamano){
      LDE_Nodo *ptr, *fin;
      LDE_Obtener(lista, &ptr);
      LDE_Fin(lista);
      LDE_Obtener(lista, &fin);
      LDE_Apuntar(lista, ptr);
      if (ptr != fin){
        ptr->posterior->anterior = n;
        n->posterior = ptr->posterior;
      } else {
        lista->fin = n;
        if (lista->circular){
          lista->inicio->anterior = n;
          n->posterior = lista->inicio;
        }
      }
      ptr->posterior = n;
      n->anterior = ptr;
    } else lista->inicio = lista->fin = lista->cursor = n;
    lista->tamano++;
    return (int) LDE_OK;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_AnadirInicio(LDE *lista, void *v){
  LDE_Inicio(lista);
  return LDE_AnadirAnterior(lista, v);
}

int LDE_AnadirFin(LDE *lista, void *v){
  LDE_Fin(lista);
  return LDE_AnadirPosterior(lista, v);
}

int LDE_Quitar(LDE *lista){
  if (lista != NULL){
    if (lista->tamano){
      void* v;
      LDE_Nodo *ptr, *inicio, *fin;
      LDE_Obtener(lista, &ptr);
      LDE_Inicio(lista);
      LDE_Obtener(lista, &inicio);
      LDE_Fin(lista);
      LDE_Obtener(lista, &fin);
      LDE_Apuntar(lista, ptr);
      v = ptr->dato;

      /* Ajustar cursor */
      if (ptr != fin) lista->cursor = ptr->posterior; /* Así, el offset se mantiene igual */
      else lista->cursor = ptr->anterior; /* En este caso, no hay posterior, el cursor ha de ir hacia atrás */

      /* Ajustar punteros */
      /* lista->circular || ptr != (inicio|fin) funciona porque en las listas circulares, el inicio y fin son solo meras etiquetas de referencia, y toda la lista es continua */
      if (lista->circular || ptr != inicio) ptr->anterior->posterior = ptr->posterior;
      else lista->inicio = ptr->posterior;
      if (lista->circular || ptr != fin) ptr->posterior->anterior = ptr->anterior;
      else lista->fin = ptr->anterior;
      free(ptr);
      lista->tamano--;

      /* Comprobar si el valor a eliminar es efectivamente nulo */
      if (v == NULL) return (int) LDE_OK;
      else return (int) LDE_ELIMINADO_NONULO;
    } else return (int) LDE_LISTA_VACIA;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_QuitarInicio(LDE *lista){
  LDE_Inicio(lista);
  return LDE_Quitar(lista);
}

int LDE_QuitarFin(LDE *lista){
  LDE_Fin(lista);
  return LDE_Quitar(lista);
}

int LDE_Posicion(LDE *lista, size_t *pos){
  if (lista != NULL){
    if (lista->tamano){
      LDE_Nodo *ptr, *meta, *fin;
      LDE_ObtenerInicio(lista, &ptr);
      LDE_ObtenerFin(lista, &fin);
      LDE_Obtener(lista, &meta);
      *pos = 0;
      while (ptr != meta){
        if (ptr == fin) return (int) LDE_CURSOR_INVALIDO;
        (*pos)++;
        ptr = ptr->posterior;
      }
      return (int) LDE_OK;
    } else return (int) LDE_LISTA_VACIA;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_Obtener(LDE *lista, LDE_Nodo **nodo){
  if (lista != NULL){
    if (lista->tamano){
      *nodo = lista->cursor;
      return (int) LDE_OK;
    } else return (int) LDE_LISTA_VACIA;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_ObtenerInicio(LDE *lista, LDE_Nodo **nodo){
  LDE_Inicio(lista);
  return LDE_Obtener(lista, nodo);
}

int LDE_ObtenerFin(LDE *lista, LDE_Nodo **nodo){
  LDE_Fin(lista);
  return LDE_Obtener(lista, nodo);
}

int LDE_Inicio(LDE *lista){
  if (lista != NULL){
    if (lista->tamano){
      lista->cursor = lista->inicio;
      return (int) LDE_OK;
    } else return (int) LDE_LISTA_VACIA;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_Fin(LDE *lista){
  if (lista != NULL){
    if (lista->tamano){
      lista->cursor = lista->fin;
      return (int) LDE_OK;
    } else return (int) LDE_LISTA_VACIA;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_ApuntarPorIndice(LDE *lista, size_t pos){
  if (lista != NULL){
    if (lista->tamano){
      if (pos<lista->tamano){ /* NOTE: && pos>=0 no es necesario, size_t es positivo */
        size_t i = 0;
        LDE_Inicio(lista);
        while (i<lista->tamano){
          if (i == pos) break;
          LDE_Avanzar(lista);
          i++;
        }
        return (int) LDE_OK;
      } else return (int) LDE_POSICION_INVALIDA;
    } else return (int) LDE_LISTA_VACIA;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_Apuntar(LDE *lista, LDE_Nodo *n){
  if (lista != NULL){
    if (lista->tamano){
      if (n != NULL){
        /* TODO: Encontrar la forma de comprobar que el nodo pertenezca a la lista. Tal vez incluyendo un puntero a la LDE. */
        lista->cursor = n;
        return (int) LDE_OK;
      } else return (int) LDE_NODO_INVALIDO;
    } else return (int) LDE_LISTA_VACIA;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_Avanzar(LDE *lista){
  if (lista != NULL){
    if (lista->tamano){
      LDE_Nodo *ptr;
      LDE_Obtener(lista, &ptr);
      /* NOTE: Esto salta a las listas circulares, en ellas no hay fin. No sé si es lo deseable. */
      if (ptr->posterior != NULL) lista->cursor = ptr->posterior;
      else return (int) LDE_LISTA_EN_FIN;
      return (int) LDE_OK;
    } else return (int) LDE_LISTA_VACIA;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_Retroceder(LDE *lista){
  if (lista != NULL){
    if (lista->tamano){
      LDE_Nodo *ptr;
      LDE_Obtener(lista, &ptr);
      /* Ídem anterior. */
      if (ptr->anterior != NULL) lista->cursor = ptr->anterior;
      else return (int) LDE_LISTA_EN_INICIO;
      return (int) LDE_OK;
    } else return (int) LDE_LISTA_VACIA;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_Vaciar(LDE *lista){
  if (lista != NULL){
    if (lista->tamano){
      do LDE_Quitar(lista); 
      while (lista->tamano);
      /*LDE_Nodo *fin, *ptr;
      LDE_Fin(lista);
      LDE_Obtener(lista, &fin);
      LDE_Inicio(lista);
      LDE_Obtener(lista, &ptr);
      while (ptr != fin) LDE_Quitar(lista);*/
    } else return (int) LDE_LISTA_VACIA;
  } else return (int) LDE_LISTA_INVALIDA;
  return (int) LDE_OK;
}

int LDE_Tamano(LDE *lista, size_t *t){
  if (lista != NULL){
    *t = lista->tamano;
    return (int) LDE_OK;
  } else return (int) LDE_LISTA_INVALIDA;
}

int LDE_PorCada(LDE *lista, int (*f)(LDE_Nodo*), int cok){
  if (lista->tamano){
    LDE_Nodo *fin, *ptr;
    LDE_Fin(lista);
    LDE_Obtener(lista, &fin);
    LDE_Inicio(lista);
    LDE_Obtener(lista, &ptr);
    f(ptr);
    do {
      int ok;
      LDE_Avanzar(lista);
      ok = f(ptr);
      if (ok != cok) return ok;
    } while (ptr != fin);
  }
  return (int) LDE_OK;
}
