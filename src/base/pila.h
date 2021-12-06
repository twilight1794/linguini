/**
 * @file pila.h
 * @brief Implementación de una pila
 * @author Giovanni Alfredo Garciliano Díaz
 * @date 4 de noviembre de 2021
 * @version 2.0
 * @copyright Licencia Pública General de GNU 3.0 o posterior
 * @warning Esta implementación guarda sólo punteros genéricos, que pueden apuntar a cualquier clase de valor. Como no guarda información sobre el tipo apuntado, la aplicación que use esta biblioteca es la responsable de saber en todo momento qué tipo de valor se guarda en la pila.
 */

#ifndef PILA_INC
#define PILA_INC

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * Códigos de operación para los métodos de Pila
 */
enum Pila_Codigos {
  PILA_OK = 100,
  PILA_PILA_INVALIDA,
  PILA_PILA_NOVACIA,
  PILA_PILA_VACIA,
  PILA_DESTINO_INVALIDO,
  PILA_ERROR_SISTEMA
};

/**
 * Pila
 */
typedef struct {
  void **lista; /**< Puntero a los elementos de la lista */
  size_t tamano; /**< Número de elementos en la pila */
} Pila;

/**
 * @brief Crea una pila
 * @return Puntero a la pila recién creada
 */
Pila* Pila_Crear();

/**
 * @brief Destruye una pila
 * @pre La pila debe estar vacía
 * @param lista Puntero a puntero a una pila
 */
int Pila_Destruir(Pila **pila);

/**
 * @brief Añade un elemento a una pila (push)
 * @param pila Puntero a una pila
 * @param e Puntero al elemento a añadir
 */
int Pila_Anadir(Pila* pila, void* e);

/**
 * @brief Quita un elemento de una pila (pop)
 * @param pila Puntero a una pila
 * @param[out] e Puntero para guardar el elemento quitado. Si el parámetro e es NULL, no se devuelve nada.
 *
 * Véase Pila_Obtener para detalles sobre el uso del parámetro e.
 */
int Pila_Quitar(Pila* pila, void** e);

/**
 * @brief Devuelve el último elemento de una pila (peek)
 * @param pila Puntero a una pila
 * @param[out] e Puntero para guardar el elemento obtenido
 * @warning A diferencia de Pila_Quitar, es fundamental que e sea un puntero válido
 * @warning e debe ser un puntero genérico. Pasar un puntero de otro tipo muy probablemente fallará. Esto no es un problema porque siempre se puede hacer un _casting_ cuando se desee usar el valor apuntado
 *
 * Este método es similar a Pila_Quitar, pero sin efectos sobre la pila
 */
int Pila_Obtener(Pila* pila, void** e);

/**
 * @brief Devuelve el tamaño de una pila
 * @param pila Puntero a una pila
 * @param[out] t Tamaño de la pila
 */
int Pila_Tamano(Pila *pila, size_t *t);

/**
 * @brief Elimina todos los elementos de una pila
 * @pre La pila no debe estar vacía
 * @param pila Puntero a una pila
 * @param f Función para eliminar apropiadamente cada elemento de la pila a eliminar.
 * Si el parámetro e es NULL, no se libera nada a eliminar
 */
int Pila_Vaciar(Pila *pila, void (*f)(void*));

#endif
