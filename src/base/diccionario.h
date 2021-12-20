/**
 * @file diccionario.h
 * @brief Implementación de una tabla asociativa de direccionamiento cerrado
 * @author Giovanni Alfredo Garciliano Díaz
 * @date 24 de noviembre de 2021
 * @version 2.0
 * @copyright Licencia Pública General de GNU 3.0 o posterior
 */

#ifndef DICC_INC
#define DICC_INC

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "lista.h"

/* 2<<(x-1) == 2^x, sin math.h */
#define POT2(x) (2<<((x)-1))

enum Dicc_Codigos {
  DICC_OK = 100,
  DICC_DICC_NOVACIO,
  DICC_DICC_VACIO,
  DICC_DICC_INVALIDO,
  DICC_LLAVE_INEXISTENTE,
  DICC_ERROR_SISTEMA
};

/**
 * Tipo para guardar valores hash e índices. Esto garantiza al menos 2^32 valores diferentes
 */
typedef unsigned long int Dicc_Hash; /* %lu */

/**
 * Entrada de un diccionario
 */
typedef struct {
  char *llave; /**< Llave usada para acceder al dato */
  Dicc_Hash hash; /**< Hash de la llave */
  void *dato; /**< Puntero al dato de una entrada */
} Dicc_Entrada;

/**
 * Diccionario
 */
typedef struct {
  LDE** entradas; /**< Lista de entradas */
  Dicc_Hash tamano; /**< Número de entradas guardadas en el diccionario */
  Dicc_Hash indices; /**< Número de índices ocupados en el diccionario */
  Dicc_Hash capacidad; /**< Potencia de 2 que expresa el número de índices que puede guardar el diccionario. Inicialmente 4 (2^4=16 índices), luego va de 4 en 4 */
} Dicc;


/**
 * @brief Crea un diccionario
 * @return Puntero a un diccionario
 */
Dicc* Dicc_Crear();

/**
 * @brief Destruye un diccionario
 * @pre El diccionario debe estar vacío
 * @param d Puntero a puntero a un diccionario
 */
int Dicc_Destruir(Dicc **d);

/**
 * @brief Añade una entrada al diccionario
 * @param d Puntero a un diccionario
 * @param l Llave a agregar
 * @param v Puntero al objeto a agregar
 */
int Dicc_Anadir(Dicc *d, char *l, void *v);

/**
 * @brief Quita una entrada del diccionario
 * @param d Puntero a un diccionario
 * @param l Llave a quitar
 */
int Dicc_Quitar(Dicc *d, char *l);

/**
 * @brief Busca una entrada en el diccionario, por su llave
 * @param d Puntero a un diccionario
 * @param l Llave a buscar
 * @return Devuelve DICC_OK si se encuentra el valor, en caso contrario devuelve DICC_LLAVE_INEXISTENTE
 */
int Dicc_Buscar(Dicc *d, char *l);

/**
 * @brief Devuelve el valor de la entrada asociada a la llave indicada
 * @pre El diccionario no debe estar vacío
 * @param d Puntero a un diccionario
 * @param l Llave a buscar
 * @param[out] nodo Puntero a puntero a donde se guardará el valor
 */
int Dicc_Obtener(Dicc *d, char *l, void **nodo);

/**
 * @brief Elimina todas las entradas de un diccionario
 * @pre El diccionario no debe estar vacío
 * @warning Los valores guardados en el diccionario deben haber ya sido destruidos apropiadamente antes de vaciar el diccionario
 * @param d Puntero a un diccionario
 */
int Dicc_Vaciar(Dicc *d);

/**
 * @brief Devuelve el número de entradas guardadas en el diccionario
 * @param d Puntero a un diccionario
 * @param[out] t Número de entradas guardadas en el diccionario
 */
int Dicc_Tamano(Dicc *d, Dicc_Hash *t);

/**
 * @brief Itera sobre las llaves de un diccionario, pasando a una función cada entrada del diccionario
 * @param d Puntero a un diccionario
 * @param f Función a ejecutar por cada nodo
 * @param cok Código de operación que indica una operación exitosa
 */
int Dicc_PorCada(Dicc *d, int (*f)(Dicc_Entrada*), int cok);

#endif
