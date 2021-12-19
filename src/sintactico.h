/**
 * @file sintactico.h
 * @brief Analizador sintáctico de Linguini
 * @author Giovanni Alfredo Garciliano Díaz
 * @date 18 de diciembre de 2021
 * @version 2.0
 * @copyright Licencia Pública General de GNU 3.0 o posterior
 */

#ifndef LING_SINTACTICO_INC
#define LING_SINTACTICO_INC

#include <ctype.h>
#include <stdbool.h>

#include <libxml/tree.h>

enum Ling_Sintactico_Codigos {
  LING_SINTACTICO_OK = 100
};

/*
 * @brief Analizador sintáctico
 * @param[in] simbolos Lista con los símbolos generados por Ling_Lexico
 * @param[out] ast Documento XML con el AST del programa
 * @param dep Bandera de depuración
 * Itera sobre un conjunto de símbolos para generar un árbol del programa
 */

int Ling_Sintactico(LDE* simbolos, xmlDocPtr ast, bool dep);

#endif