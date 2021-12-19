/**
 * @file lexico.h
 * @brief Analizador léxico de Linguini
 * @author Giovanni Alfredo Garciliano Díaz
 * @date 18 de diciembre de 2021
 * @version 2.0
 * @copyright Licencia Pública General de GNU 3.0 o posterior
 */

#ifndef LING_LEXICO_INC
#define LING_LEXICO_INC

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

enum Ling_Lexico_Codigos {
  LING_LEXICO_OK = 100,
  LING_LEXICO_CARACTER_ILEGAL
};


typedef enum {
  SIMBOLO_SENTENCIA = 1,
  SIMBOLO_EXPRESION
} Ling_Lexico_Tiposym;

/*
 * @brief Analizador léxico
 * @param[in] archivo Archivo que contiene el código fuente del programa
 * @param[out] simbolos Lista con los símbolos generados
 * @param dep Bandera de depuración
 * Lee caracteres del código fuente y genera una lista de símbolos.
 */

int Ling_Lexico(FILE *archivo, LDE* simbolos, bool dep);

#endif