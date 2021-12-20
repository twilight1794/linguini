/**
 * @file recolector.h
 * @brief Administrador de la memoria de Linguini (i.e. recolector de basura)
 * @author Giovanni Alfredo Garciliano Díaz
 * @date 19 de diciembre de 2021
 * @version 2.0
 * @copyright Licencia Pública General de GNU 3.0 o posterior
 */

#ifndef RECOLECTOR_INC
#define RECOLECTOR_INC

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

enum Recolector_Codigos {
  RECOLECTOR_OK = 100,
  RECOLECTOR_INDICE_ERRONEO,
  RECOLECTOR_ERROR_SISTEMA
}

/**
 * Lista de objetos
 */
typedef struct {
  Remy_Objeto* vals;
  size_t elems;
} Remy_Objetos;

/**
 * Información sobre un objeto
 */
typedef struct {
  void* val; /**< Puntero al objeto */
  size_t refs; /**< Número de referencias */
} Remy_Objeto;


/**
 * @brief Crea una lista de objetos
 */
Remy_Objetos* Recolector_Crear();

/**
 * @brief Destruye una lista de objetos
 */
int Recolector_Destruir(Remy_Objetos** r);

/**
 * @brief Añade un objeto a la lista de objetos
 */
int Recolector_Anadir(Remy_Objetos* r, void* o);

/**
 * @brief Actualiza el contador de referencias de un objeto de la lista de objetos
 */
int Recolector_Actualizar(Remy_Objetos* r, size_t p, int op);

/**
 * @brief Elimina un objeto de la lista de objetos
 */
int Recolector_Eliminar(Remy_Objetos* r, size_t p);

/**
 * @brief Analiza una lista de objetos para eliminar los elementos que ya no están en uso
 */
int Recolector_Limpiar(Remy_Objetos* r);

#endif