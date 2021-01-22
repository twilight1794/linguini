// Utilidades ajenas a Linguini
#include <stdlib.h>
#include <string.h>

// Implementación de booleanos
typedef unsigned int bool;
bool true = 1;
bool false = 0;

// Manipulación de cadenas variables
/*
 * Asigna una porción de memoria dinámica
 */
char *iniciarCadena(){
  char *cad = malloc(1);
  if (*cad) cad[0] = 0x00;
  return cad;
}

/*
 * Aumenta en un byte la memoria asignada a la cadena
 */
int anadirCadena(char **cad, char c){
  char ap[2] = {c, 0x00};
  size_t t = strlen(*cad);
  *cad = realloc(*cad, t+2);
  if (!(*cad)) return 400;
  strncat(*cad, ap, 1);
  return 0;
}

/*
 * Concatena dos cadenas usando strncat
 */
int anadirSCadena(char **cad, char *c){
  size_t t = strlen(*cad) + strlen(c);
  *cad = realloc(*cad, t+1);
  strncat(*cad, c, strlen(c));
  return 0;
}

/*
 * Reduce en un byte la memoria asignada a la cadena
 */
int quitarCadena(char **cad){
  size_t t = strlen(*cad);
  char *temp = (char*) malloc(t);
  strncpy(temp, *cad, t-1);
  free(*cad);
  *cad = temp;
  return 0;
}

// Arrays dinámicos genéricos
typedef struct {
  int tamano;
  int *tipos;
  void **valores;
} Array;

/*
 * Inicializa un array
 */
Array iniciarArray(){
  Array a;
  a.tamano = 0;
  a.tipos = NULL;
  a.valores = NULL;
  return a;
}

void anadirArray(Array *a, void *e, int t){
  //printf("ArrayAdd: %s\n", (char*) e);
  size_t ntv = sizeof(a->valores)*(size_t) a->tamano+sizeof(a->valores);
  a->valores = realloc(a->valores, ntv);
  a->valores[a->tamano] = e; // a->tamano no es el tamaño actual, sino el anterior a la expansión, lo que da el offset correcto
  size_t ntt = sizeof(int)* (size_t) a->tamano+sizeof(int);
  a->tipos = realloc(a->tipos, ntt);
  a->tipos[a->tamano] = t; // Idem
  a->tamano++;
  //printf("ArrayAdd2: %s\n", (char*) a->valores[a->tamano-1]);
}

void quitarArray(Array *a){
  size_t ntv = sizeof(a->valores)* (size_t) a->tamano-sizeof(a->valores);
  a->valores = realloc(a->valores, ntv);
  size_t ntt = sizeof(int)* (size_t) a->tamano-sizeof(int);
  a->tipos = realloc(a->tipos, ntt);
  a->tamano--;
}

void eliminarArray(Array *a){
  free(a->tipos);
  free(a->valores);
}

/*
 * Itera sobre un vector de cadenas de texto buscando una coincidencia
 * Análogo de las estructuras ... in <Array>
 * Devuelve la posición encontrada, o -1 si no hay coincidencias
 * @param cad 
 * @param vector 
 */
int enArray(const char *cad, const Array *vector){
  int i;
  for(i = 0; i<vector->tamano; i++) {
    //if(!strncmp(vector->valores[i], cad, strlen(cad))){
    if(!strcmp(vector->valores[i], cad)) {
      return i;
    }
  }
  return -1;
}

int enEscalar(char **arr, int len, char *target) {
  int i;
  for(i = 0; i<len; i++) {
    //if(strncmp(arr[i], target, strlen(target)) == 0){
    if(!strcmp(arr[i], target)){
      return i;
    }
  }
  return -1;
}

// Diccionario simple
typedef struct {
  Array identificadores;
  Array valores;
} Dict;

/*
 * Inicializa un diccionario
 */
Dict iniciarDict(){
  Dict a;
  a.identificadores = iniciarArray();
  a.valores = iniciarArray();
  return a;
}

/*
 * Añade un elemento al diccionario
 */
void anadirDict(Dict *a, char *id, void *e, int t){
  anadirArray(&a->valores, e, t);
  anadirArray(&a->identificadores, id, 0);
}

/*
 * Obtiene un valor de un diccionario, dada su clave
 */
void obtenerValorDict(Dict *a, const char *clave, void **ant){
  int pos = enArray(clave, &a->identificadores);
  if (pos < 0) *ant = NULL;
  else *ant = a->valores.valores[pos];
}
