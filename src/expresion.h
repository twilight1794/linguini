/**
 * @file expresion.h
 * @brief Analizador de expresiones
 * @author Giovanni Alfredo Garciliano Díaz
 * @date 13 de agosto de 2021
 * @version 2.0
 * @copyright Licencia Pública General de GNU 3.0 o posterior
 */

#ifndef EXPR_INC
#define EXPR_INC

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>

#include "lista.h"
#include "utilidades.h"

enum Expr_Codigos {
  EXPR_OK = 100,
  EXPR_CARACTER_ILEGAL,
  EXPR_NUMERO_MALFORMADO,
  EXPR_EXPRESION_MALFORMADA,
  EXPR_PARENTESIS_EXCEDENTE
};

enum Expr_Tipos {
  /* Inicialización */
  VALOR_INDEFINIDO = 0,
  /* Análisis léxico */
  SIMB_NUMERO, SIMB_IDENTIFICADOR, SIMB_OPERADOR, SIMB_CADENA, SIMB_LOGICO,
  /* Análisis sintáctico = 6 */
  CONSTANTE_ENTERA, CONSTANTE_DECIMAL, CONSTANTE_CADENA, CONSTANTE_LOGICA, IDENTIFICADOR, IDENTIFICADOR_FUNCION, OPERADOR_SUBEXP_A, OPERADOR_SUBEXP_C, OPERADOR_AMBITO, OPERADOR_LLAMADA_A, OPERADOR_LLAMADA_C, OPERADOR_POTENCIA, OPERADOR_RAIZ, OPERADOR_MULTIPLICACION, OPERADOR_DIVISION, OPERADOR_MODULO, OPERADOR_SUMA, OPERADOR_RESTA, OPERADOR_MAYOR, OPERADOR_MENOR, OPERADOR_MAYOR_IGUAL, OPERADOR_MENOR_IGUAL, OPERADOR_IGUAL, OPERADOR_DISTINTO, OPERADOR_Y, OPERADOR_O, OPERADOR_SEPARADOR
};

/**
 * @brief Estructura para guardar símbolos y elementos de expresiones
 */
typedef struct {
  int tipo; /**< Tipo de símbolo */
  union {
    unsigned char *Cad; /**< Cadenas, identificadores, genérico para símbolos */
    long int Ent; /**< Enteros */
    long double Dec; /**< Flotantes */
    bool Bool; /**< Booleanos  */
    int Subtipo; /**< Para otros subtipos */
  } val;
  size_t params; /**< Para funciones, número de argumentos recibidos */
} Expr_Elem;

/**
 * @brief Indica si un operador tiene mayor, menor o igual precedencia que otro.
 * @param[in] a Código de un operador
 * @param[in] b Código de un operador
 * @return Devuelve 1 si a > b, 0 si son iguales, -1 si a < b, y 2 en caso de error.
 */
int Expr_Precedencia(int a,  int b);

/**
 * @brief Analiza una expresión y genera los símbolos de la expresión
 * @param[in] e Cadena de texto que contiene una expresión en formato infijo
 * @param[out] simbolos Lista donde se guardarán los símbolos
 * @warning simbolos ya debe estar inicializada antes de ser pasada a la función
 */
int Expr_ALexico(const unsigned char *e, LDE* simbolos);

/**
 * @brief Convierte los símbolos de la expresión a objetos, y los ordena en RPN
 * @param[in] simbolos Símbolos de la expresión generados por alExpresion
 * @param[out] expresion Lista que contiene objetos de expresión en notación polaca inversa
 * @warning expresion ya debe estar inicializada antes de ser pasada a la función
 */
int Expr_ASintactico(LDE* simbolos, LDE* expresion);

#endif
