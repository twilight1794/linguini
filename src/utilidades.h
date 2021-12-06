/**
 * @file utilidades.h
 * @brief Estructuras varias
 */

#ifndef UTILS_INC
#define UTILS_INC

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "lista.h"

/*bool is_printable_ascii = (ch & ~0x7f) == 0 && (isprint() || isspace()) ;*/

/**
 * Códigos de operación para los métodos de utilidades.h
 */
enum Utils_Codigos {
  UTILS_OK = 100,
  UTILS_OP_FALLIDA,
  CADENA_CARACTER_INVALIDO,
  CADENA_CADENA_INVALIDA,
  DICC_DICCIONARIO_INVALIDO,
  DICC_DICCIONARIO_NOVACIO,
  DICC_DICCIONARIO_VACIO,
  DICC_IDENTIFICADOR_INVALIDO
};

/*
 * Cadenas de texto UTF-8 variables
 */

/**
 * @brief Crea una cadena de texto vacía
 * @return Puntero a una cadena
 */
unsigned char* Cadena_Crear(void);

/**
 * @brief Añade un caracter a la cadena de caracteres
 * @param cad Cadena de texto a manipular
 * @param c Puntero al caracter a añadir
 */
int Cadena_Anadir(unsigned char **cad, const unsigned char *c);

/**
 * @brief Concatena una cadena a la cadena de caracteres
 * @param cad Cadena de texto a manipular
 * @param c Cadena de texto a añadir
 */
int Cadena_Concatenar(unsigned char **cad, unsigned char *c);

/**
 * @brief Quita un caracter de la cadena de caracteres
 * @param cad Cadena de texto a manipular
 */
int Cadena_Quitar(unsigned char **cad);

/**
 * @brief Indica el tamaño de una cadena de caracteres
 * @param cad Cadena de texto a manipular
 */
int Cadena_Tamano(unsigned char *cad, size_t *t);

/**
 * @brief Dado un caracter, indica cuántos bytes ocupa si es codificado en UTF-8
 * @param c Puntero a caracter
 */
size_t Cadena_tamanoCaracter(const unsigned char *c);

/**
 * Diccionario
 */
typedef struct {
  LDE* identificadores; /**< Array que contiene los identificadores de los valores **/
  LDE* valores; /**< Array que contiene los punteros a los valores **/
} Dicc;

/**
 * @brief Crea un diccionario
 * @return Puntero a un diccionario
 */
Dicc* Dicc_Crear(void);

/**
 * @brief Destruye un diccionario
 * @pre El diccionario debe estar vacío
 * @param dicc Puntero a puntero a un diccionario
 */
int Dicc_Destruir(Dicc **dicc);

/**
 * @brief Añade un elemento al diccionario
 * @param dicc Puntero a diccionario
 * @param id Identificador del valor
 * @param v Puntero al objeto a agregar
 * @warning Si ya hay un valor identificado por la clave `id`, será reemplazado
 */
int Dicc_Anadir(Dicc *dicc, unsigned char *id, void *v);

/**
 * @brief Quita un elemento del diccionario
 * @pre La clave debe existir en el diccionario
 * @param dicc Puntero a diccionario
 * @param id Identificador del valor
 */
int Dicc_Quitar(Dicc *dicc, unsigned char *id);

/**
 * @brief Devuelve un valor del diccionario
 * @pre La clave debe existir en el diccionario
 * @param dicc Puntero a diccionario
 * @param id Identificador del valor
 * @param[out] valor Puntero a puntero a donde se guardará el valor
 */
int Dicc_Obtener(Dicc *dicc, unsigned char *id, void **valor);

/**
 * @brief Elimina todos los valores de un diccionario
 * @pre El diccionario no debe estar vacío
 * @warning Los valores guardados en el diccionario deben haber ya sido destruidos apropiadamente antes de vaciar el diccionario
 * @param dicc Puntero a diccionario
 */
int Dicc_Vaciar(Dicc *dicc);

/**
 * @brief Devuelve el tamaño de un diccionario
 * @param dicc Puntero a diccionario
 * @param[out] t Tamaño del diccionario
 */
int Dicc_Tamano(Dicc *dicc, size_t *t);

#endif
