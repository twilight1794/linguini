/**
 * @file cadena.h
 * @brief Cadenas de texto UTF-8 variables
 * @author Giovanni Alfredo Garciliano Díaz
 * @date 18 de diciembre de 2021
 * @version 2.0
 * @copyright Licencia Pública General de GNU 3.0 o posterior
 */

#ifndef UTILS_INC
#define UTILS_INC

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>


enum Cadena_Codigos {
  CADENA_OK = 100,
  CADENA_OP_FALLIDA,
  CADENA_CARACTER_INVALIDO,
  CADENA_CADENA_INVALIDA,
};

typedef unsigned char* Cadena; 

/**
 * @brief Crea una cadena de texto vacía
 * @return Puntero a una cadena
 */
Cadena Cadena_Crear(void);

/**
 * @brief Añade un caracter a la cadena de caracteres
 * @param cad Cadena de texto a manipular
 * @param c Puntero al caracter a añadir
 */
int Cadena_Anadir(Cadena *cad, const Cadena c);

/**
 * @brief Concatena una cadena a la cadena de caracteres
 * @param cad Cadena de texto a manipular
 * @param c Cadena de texto a añadir
 */
int Cadena_Concatenar(Cadena *cad, Cadena c);

/**
 * @brief Quita un caracter de la cadena de caracteres
 * @param cad Cadena de texto a manipular
 */
int Cadena_Quitar(Cadena *cad);

/**
 * @brief Indica el tamaño de una cadena de caracteres
 * @param cad Cadena de texto a manipular
 */
int Cadena_Tamano(Cadena cad, size_t *t);

/**
 * @brief Dado un caracter, indica cuántos bytes ocupa si es codificado en UTF-8
 * @param c Puntero a caracter
 */
size_t Cadena_tamanoCaracter(const Cadena c);

#endif