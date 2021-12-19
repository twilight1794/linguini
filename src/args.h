/**
 * @file args.h
 * @brief Procesa los argumentos de Linguini por línea de comandos
 * @author Giovanni Alfredo Garciliano Díaz
 * @date 17 de diciembre de 2021
 * @version 1.0
 * @copyright Licencia Pública General de GNU 3.0 o posterior
 */

#ifndef ARGS_INC
#define ARGS_INC

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "diccionario.h"
#include "lista.h"
#include "tiposdin.h"

enum Args_Codigos {
  ARGS_OK = 100,
  ARGS_NOPARAM,
  ARGS_PARAM_DESCONOCIDO,
  ARGS_PARAM_INCOMPLETO,
  ARGS_VAL_F_INVALIDO
};

/**
 * @brief Devuelve un diccionario con los argumentos procesados
 * @param[in] argc Número de argumentos pasados por consola
 * @param[in] argv Arreglo de cadenas de texto que contiene los argumentos pasados por consola
 * @param[out] d Diccionario donde se escribirán los datos
 * argc y argv casi siempre corresponden a los parámetros homónimos de la función de entrada
 */
int Args_Procesar(int argc, char** argv, Dicc **d);

#endif
