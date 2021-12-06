/**
 * @file lista.h
 * @brief Implementación de una lista doblemente enlazada
 * @author Giovanni Alfredo Garciliano Díaz
 * @date 23 de octubre de 2021
 * @version 2.0
 * @copyright Licencia Pública General de GNU 3.0 o posterior
 * La biblioteca implementa en la misma estructura tanto una lista lineal como una circular.
 */

#ifndef LDE_INC
#define LDE_INC

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * Códigos de operación para los métodos de LDE
 */
enum LDE_Codigos {
  LDE_OK = 100,
  LDE_LISTA_INVALIDA,
  LDE_LISTA_NOVACIA,
  LDE_LISTA_VACIA,
  LDE_ERROR_SISTEMA,
  LDE_NODO_INVALIDO,
  LDE_POSICION_INVALIDA,
  LDE_LISTA_EN_INICIO,
  LDE_LISTA_EN_FIN,
  LDE_CURSOR_INVALIDO, /* El cursor apunta a un nodo que no pertenece a la lista enlazada */
  LDE_ELIMINADO_NONULO /* Esto es más una advertencia que un error en sí */
};

/**
 * Nodo de una LDE
 */
typedef struct LDE_Nodo {
  void *dato; /**< Puntero al dato de un nodo */
  struct LDE_Nodo *anterior; /**< Puntero al nodo anterior. Nil si es el primer nodo. */
  struct LDE_Nodo *posterior; /**< Puntero al nodo posterior. Nil si es el último nodo. */
} LDE_Nodo;

/**
 @brief Crea un nodo nuevo
 @param x Puntero al valor a agregar al nodo
 @return Puntero al nodo recién creado
 */
LDE_Nodo* LDE_Nodo_Crear(void *x);

/**
 * @brief Devuelve el valor de un nodo
 * @param n Puntero a un nodo
 * @param[out] v Puntero a puntero a donde se guardará el valor obtenido
 */
int LDE_Nodo_Obtener(LDE_Nodo *n, void *v);

/**
 * @brief Asigna un valor a un nodo
 * @param n Puntero a un nodo
 * @param v Puntero al valor a asignar
 */
int LDE_Nodo_Poner(LDE_Nodo *n, void* v);


/**
 * Lista doblemente enlazada
 */
typedef struct {
  LDE_Nodo *inicio; /**< Primer nodo de la lista */
  LDE_Nodo *fin; /**< Último nodo de la lista */
  LDE_Nodo *cursor; /**< Puntero al nodo sobre el cual los métodos de LDE operarán */
  bool circular; /**< Indica si la LDE es circular. */
  size_t tamano; /**< Número de nodos en la lista */
} LDE;

/**
 * @brief Crea una lista doblemente enlazada
 * @param circular Indica verdadero si la lista es circular
 * @return Puntero a una lista
 */
LDE* LDE_Crear(bool circular);

/**
 * @brief Destruye una lista doblemente enlazada
 * @pre La lista debe estar vacía
 * @param lista Puntero a puntero a una LDE
 */
int LDE_Destruir(LDE **lista);


/**
 * @brief Añade un objeto a una LDE, antes del objeto apuntado por el cursor
 * @param lista Puntero a una LDE
 * @param v Puntero al objeto a agregar
 */
int LDE_AnadirAnterior(LDE *lista, void *v);

/**
 * @brief Añade un objeto a una LDE, después del objeto apuntado por el cursor
 * @param lista Puntero a una LDE
 * @param v Puntero al objeto a agregar
 */
int LDE_AnadirPosterior(LDE *lista, void *v);

/**
 * @brief Añade un objeto al inicio de una LDE
 * @param lista Puntero a una LDE
 * @param v Puntero al objeto a agregar
 */
int LDE_AnadirInicio(LDE *lista, void *v);

/**
 * @brief Añade un objeto al final de una LDE
 * @param lista Puntero a una LDE
 * @param v Puntero al objeto a agregar
 */
int LDE_AnadirFin(LDE *lista, void *v);


/**
 * @brief Quita un objeto a una LDE, que es apuntado por el cursor
 * @pre La LDE no debe estar vacía
 * @param lista Puntero a una LDE
 */
int LDE_Quitar(LDE *lista);

/**
 * @brief Quita el objeto al inicio de una LDE
 * @pre La LDE no debe estar vacía
 * @param lista Puntero a una LDE
 */
int LDE_QuitarInicio(LDE *lista);

/**
 * @brief Quita el objeto al final de una LDE
 * @pre La LDE no debe estar vacía
 * @param lista Puntero a una LDE
 */
int LDE_QuitarFin(LDE *lista);


/**
 * @brief Devuelve la posición del nodo apuntado por el cursor de una LDE
 * @pre La LDE no debe estar vacía
 * @param lista Puntero a una LDE
 * @param[out] pos Puntero a objeto size_t donde se guardará la posición del nodo
 */
int LDE_Posicion(LDE *lista, size_t *pos);

/**
 * @brief Devuelve el nodo apuntado por el cursor de una LDE
 * @pre La LDE no debe estar vacía
 * @param lista Puntero a una LDE
 * @param[out] nodo Puntero a puntero a donde se guardará el nodo
 */
int LDE_Obtener(LDE *lista, LDE_Nodo **nodo);

/**
 * @brief Devuelve el nodo al inicio de una LDE
 * @pre La LDE no debe estar vacía
 * @param lista Puntero a una LDE
 * @param[out] nodo Puntero a puntero a donde se guardará el nodo
 */
int LDE_ObtenerInicio(LDE *lista, LDE_Nodo **nodo);

/**
 * @brief Devuelve el nodo al final de una LDE
 * @pre La LDE no debe estar vacía
 * @param lista Puntero a una LDE
 * @param[out] nodo Puntero a puntero a donde se guardará el nodo
 */
int LDE_ObtenerFin(LDE *lista, LDE_Nodo **nodo);

/**
 * @brief Mueve el cursor hacia el inicio de una LDE
 * @pre La LDE no debe estar vacía
 * @param lista Puntero a una LDE
 */
int LDE_Inicio(LDE *lista);

/**
 * @brief Mueve el cursor hacia el fin de una LDE
 * @pre La LDE no debe estar vacía
 * @param lista Puntero a una LDE
 */
int LDE_Fin(LDE *lista);

/**
 * @brief Mueve el cursor hacia la posición indicada en una LDE
 * @pre La LDE no debe estar vacía
 * @param lista Puntero a una LDE
 * @param pos Posición a la cual apuntar el cursor, contando desde 0
 */
int LDE_ApuntarPorIndice(LDE *lista, size_t pos);

/**
 * @brief Mueve el cursor de una LDE hacia un nodo en particular
 * @pre La LDE no debe estar vacía
 * @pre El nodo debe existir en la lista a modificar. la API no hará una comprobación y la tarea no fallará si el nodo no pertenece a la lista enlazada, pero desde entonces el cursor quedará atrapado en los nodos de otra lista enlazada.
 * @param lista Puntero a una LDE
 * @param n Nodo al cual apuntar
 */
int LDE_Apuntar(LDE *lista, LDE_Nodo *n);

/**
 * @brief Mueve el cursor una posición adelante en una LDE
 * @pre La LDE no debe estar vacía
 * @param lista Puntero a una LDE
 */
int LDE_Avanzar(LDE *lista);

/**
 * @brief Mueve el cursor una posición atrás en una LDE
 * @pre La LDE no debe estar vacía
 * @param lista Puntero a una LDE
 */
int LDE_Retroceder(LDE *lista);

/**
 * @brief Elimina todos los nodos de una LDE
 * @pre La LDE no debe estar vacía
 * @warning Los valores guardados en la LDE deben haber ya sido destruidos apropiadamente antes de vaciar la LDE
 * @param lista Puntero a una LDE
 */
int LDE_Vaciar(LDE *lista);

/**
 * @brief Devuelve el tamaño de una LDE
 * @param lista Puntero a una LDE
 * @param[out] t Tamaño de la LDE
 */
int LDE_Tamano(LDE *lista, size_t *t);

/**
 * @brief Itera sobre una LDE, pasando a una función cada nodo de la lista
 * @param lista Puntero a una LDE
 * @param f Función a ejecutar por cada nodo
 * @param cok Código de operación que indica una operación exitosa
 */
int LDE_PorCada(LDE *lista, int (*f)(LDE_Nodo*), int cok);

#endif
