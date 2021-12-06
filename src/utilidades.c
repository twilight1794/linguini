#include "utilidades.h"

unsigned char* Cadena_Crear(void){
  unsigned char *cad = malloc(1);
  if (cad != NULL) cad[0] = '\0';
  return cad;
}

int Cadena_Anadir(unsigned char **cad, const unsigned char *c){
  size_t tcar, tcad, i = 0;
  if (*cad == NULL || c == NULL) return (int) CADENA_CADENA_INVALIDA;
  tcar = Cadena_tamanoCaracter(c);
  Cadena_Tamano(*cad, &tcad);
  *cad = realloc(*cad, tcad + tcar + 1);
  if (*cad == NULL) return (int) UTILS_OP_FALLIDA;
  while (i<tcar){
    (*cad)[tcad+i] = c[i];
    i++;
  }
  return (int) UTILS_OK;
}

int Cadena_Concatenar(unsigned char **cad, unsigned char *c){
  unsigned char *pCad, *pC;
  size_t tcad, tcar;
  if (*cad == NULL || c == NULL) return (int) CADENA_CADENA_INVALIDA;
  Cadena_Tamano(*cad, &tcad);
  pCad = *cad + tcad; /* Puntero al final de la cadena receptora */
  pC = c;
  while (*pC != '\0'){
    tcar = Cadena_tamanoCaracter(pC);
    *cad = realloc(*cad, tcad + tcar + 1);
    if (*cad == NULL) return (int) UTILS_OP_FALLIDA;
    while (tcar > 0){
      *pCad = *pC;
      pCad++;
      pC++;
      tcar--;
    };
    Cadena_Tamano(*cad, &tcad); /* Actualiza el tamaño de la cadena receptora para la siguiente iteración */
  }
  return (int) UTILS_OK;
}

int Cadena_Quitar(unsigned char **cad){
  size_t tcad, i = 0;
  if (*cad == NULL) return (int) CADENA_CADENA_INVALIDA;
  Cadena_Tamano(*cad, &tcad);
  do {
    i++;
  } while((*(*cad + (tcad-i)) & 0xC0) == 0x80);
  *cad = realloc(*cad, tcad-i+1);
  if (*cad == NULL) return (int) UTILS_OP_FALLIDA;
  (*cad)[tcad-i] = '\0';
  return (int) UTILS_OK;
}

int Cadena_Tamano(unsigned char *cad, size_t *t){
  *t = 0;
  if (cad != NULL){
    while (*cad != '\0'){
      cad++;
      (*t)++;
    }
    return (int) UTILS_OK;
  } else return (int) CADENA_CADENA_INVALIDA;
}

size_t Cadena_tamanoCaracter(const unsigned char *c){
  size_t n;
  if (*c == 0xC0 || *c == 0xC1 || *c >= 0xF5) n=0;
  else if (*c >= 0xF0) n=4;
  else if (*c >= 0xE0) n=3;
  else if (*c >= 0xC2) n=2;
  else n=1;
  return n;
}

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
