#include "utilidades.h"

Dicc* Dicc_Crear(void){
  Dicc* dicc = (Dicc*) malloc(sizeof(Dicc));
  if (dicc != NULL){
    dicc->identificadores = LDE_Crear();
    dicc->valores = LDE_Crear();
  }
  return dicc;
}

int Dicc_Destruir(Dicc **dicc){
  if (*dicc != NULL){
    if (!((*dicc)->valores->tamano)){
      free(*dicc);
      *dicc = NULL;
      return (int) UTILS_OK;
    } else return (int) DICC_DICCIONARIO_NOVACIO;
  } else return (int) DICC_DICCIONARIO_INVALIDO;
}

int Dicc_Anadir(Dicc *dicc, unsigned char *id, void *v){
  if (dicc != NULL){
    int a, b;
    a = LDE_AnadirPosterior(dicc->identificadores, id);
    if (a != LDE_OK) return a;
    LDE_Fin(dicc->identificadores);
    b = LDE_AnadirPosterior(dicc->valores, v);
    if (b != LDE_OK) return b;
    LDE_Fin(dicc->valores);
    return (int) UTILS_OK;
  }
  return (int) DICC_DICCIONARIO_INVALIDO;
}

int Dicc_Quitar(Dicc *dicc, unsigned char *id){
  if (dicc != NULL){
    if (dicc->valores->tamano){
      LDE_Nodo *ptr, *fin;
      unsigned char *val;
      size_t i = 0;
      LDE_Fin(dicc->identificadores);
      LDE_Obtener(dicc->identificadores, &fin);
      LDE_Inicio(dicc->identificadores);
      do {
        LDE_Obtener(dicc->identificadores, &ptr);
        LDE_Nodo_Obtener(ptr, (void **) &val);
        if (!strcmp((char *) val, (char *) id)){
          LDE_Quitar(dicc->identificadores);
          LDE_Mover(dicc->valores, i);
          LDE_Quitar(dicc->valores);
          return (int) UTILS_OK;
        }
        LDE_Avanzar(dicc->identificadores);
        i++;
      } while (ptr != fin);
      return (int) DICC_IDENTIFICADOR_INVALIDO;
    } else return (int) DICC_DICCIONARIO_VACIO;
  }
  return (int) DICC_DICCIONARIO_INVALIDO;
}

int Dicc_Obtener(Dicc *dicc, unsigned char *id, void **valor){
  if (dicc != NULL){
    if (dicc->valores->tamano){
      LDE_Nodo *ptr, *fin;
      unsigned char *val;
      size_t i = 0;
      LDE_Fin(dicc->identificadores);
      LDE_Obtener(dicc->identificadores, &fin);
      LDE_Inicio(dicc->identificadores);
      do {
        LDE_Obtener(dicc->identificadores, &ptr);
        LDE_Nodo_Obtener(ptr, (void **) &val);
        if (!strcmp((char *) val, (char *) id)){
          LDE_Mover(dicc->valores, i);
          LDE_Obtener(dicc->valores, &ptr);
          LDE_Nodo_Obtener(ptr, *valor);
          return (int) UTILS_OK;
        }
        LDE_Avanzar(dicc->identificadores);
        i++;
      } while (ptr != fin);
      return (int) DICC_IDENTIFICADOR_INVALIDO;
    } else return (int) DICC_DICCIONARIO_VACIO;
  }
  return (int) DICC_DICCIONARIO_INVALIDO;
}

int Dicc_Vaciar(Dicc *dicc){
  if (dicc != NULL){
    int a, b;
    a = LDE_Vaciar(dicc->identificadores);
    if (a != LDE_OK) return a;
    b = LDE_Vaciar(dicc->valores);
    if (b != LDE_OK) return b;
    return (int) UTILS_OK;
  }
  return (int) DICC_DICCIONARIO_INVALIDO;
}

int Dicc_Tamano(Dicc *dicc, size_t *t){
  if (dicc != NULL){
    LDE_Tamano(dicc->valores, t);
    return (int) UTILS_OK;
  } else return (int) DICC_DICCIONARIO_INVALIDO;
}
