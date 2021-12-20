#include "cadena.h"

unsigned char* Cadena_Crear(void){
  unsigned char *cad = malloc(1);
  if (cad != NULL) cad[0] = '\0';
  return cad;
}

int Cadena_Anadir(Cadena *cad, const Cadena c){
  size_t tcar, tcad, i = 0;
  if (*cad == NULL || c == NULL) return (int) CADENA_CADENA_INVALIDA;
  tcar = Cadena_tamanoCaracter(c);
  Cadena_Tamano(*cad, &tcad);
  *cad = realloc(*cad, tcad + tcar + 1);
  if (*cad == NULL) return (int) CADENA_OP_FALLIDA;
  while (i<tcar){
    (*cad)[tcad+i] = c[i];
    i++;
  }
  return (int) CADENA_OK;
}

int Cadena_Concatenar(Cadena *cad, const Cadena c){
  unsigned char *pCad, *pC;
  size_t tcad, tcar;
  if (*cad == NULL || c == NULL) return (int) CADENA_CADENA_INVALIDA;
  Cadena_Tamano(*cad, &tcad);
  pCad = *cad + tcad; /* Puntero al final de la cadena receptora */
  pC = c;
  while (*pC != '\0'){
    tcar = Cadena_tamanoCaracter(pC);
    *cad = realloc(*cad, tcad + tcar + 1);
    if (*cad == NULL) return (int) CADENA_OP_FALLIDA;
    while (tcar > 0){
      *pCad = *pC;
      pCad++;
      pC++;
      tcar--;
    };
    Cadena_Tamano(*cad, &tcad); /* Actualiza el tamaño de la cadena receptora para la siguiente iteración */
  }
  return (int) CADENA_OK;
}

int Cadena_Quitar(Cadena *cad){
  size_t tcad, i = 0;
  if (*cad == NULL) return (int) CADENA_CADENA_INVALIDA;
  Cadena_Tamano(*cad, &tcad);
  do {
    i++;
  } while((*(*cad + (tcad-i)) & 0xC0) == 0x80);
  *cad = realloc(*cad, tcad-i+1);
  if (*cad == NULL) return (int) CADENA_OP_FALLIDA;
  (*cad)[tcad-i] = '\0';
  return (int) CADENA_OK;
}

int Cadena_Tamano(Cadena cad, size_t *t){
  *t = 0;
  if (cad != NULL){
    while (*cad != '\0'){
      cad++;
      (*t)++;
    }
  } else return (int) CADENA_CADENA_INVALIDA;
  return (int) CADENA_OK;
}

size_t Cadena_tamanoCaracter(const Cadena c){
  size_t n;
  if (*c == 0xC0 || *c == 0xC1 || *c >= 0xF5) n=0;
  else if (*c >= 0xF0) n=4;
  else if (*c >= 0xE0) n=3;
  else if (*c >= 0xC2) n=2;
  else n=1;
  return n;
}