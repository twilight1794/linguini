/**
 *@file promesa.h
 * @brief Implementación de un sistema para devolver códigos de operación y valores juntos
 */

#ifndef PROM_INC
#define PROM_INC

#include <stdlib.h>

/**
 * Promesa
 */
typedef struct {
  int codigo; /**< Código de operación a devolver */
  union {
    void* valp; /**< Valor puntero */
    int vali; /**< Valor entero */
    char valc; /**< Valor caracter */
    float valf; /**< Valor flotante */
  } val; /**< Unión para poder devolver varios tipos de valor en la misma estructura */
} Promesa;

/**
 * @brief Pone un código a una Promesa
 * @param[out] p Puntero a Promesa
 * @param[in] c Código entero a poner
 */
void Promesa_PonerCodigo(Promesa *p, int c);

/**
 * @brief Pone el puntero a retornar a una Promesa
 * @param[out] p Puntero a Promesa
 * @param[in] v Valor puntero a retornar
 */
void Promesa_PonerValorP(Promesa *p, void* v);

/**
 * @brief Pone el entero a retornar a una Promesa
 * @param[out] p Puntero a Promesa
 * @param[in] v Valor entero a retornar
 */
void Promesa_PonerValorI(Promesa *p, int v);

/**
 * @brief Pone el caracter a retornar a una Promesa
 * @param[out] p Puntero a Promesa
 * @param[in] v Valor caracter a retornar
 */
void Promesa_PonerValorC(Promesa *p, char v);

/**
 * @brief Pone el flotante a retornar a una Promesa
 * @param[out] p Puntero a Promesa
 * @param[in] v Valor flotante a retornar
 */
void Promesa_PonerValorF(Promesa *p, float v);

/**
 * @brief Devuelve el código de operación devuelto por la función
 * @param[in] p Puntero a Promesa
 */
int Promesa_Codigo(Promesa *p);

/**
 * @brief Devuelve el valor puntero devuelto por la función
 * @param[in] p Puntero a Promesa
 */
void* Promesa_ValorP(Promesa *p);

/**
 * @brief Devuelve el valor entero devuelto por la función
 * @param[in] p Puntero a Promesa
 */
int Promesa_ValorI(Promesa *p);

/**
 * @brief Devuelve el valor caracter devuelto por la función
 * @param[in] p Puntero a Promesa
 */
char Promesa_ValorC(Promesa *p);

/**
 * @brief Devuelve el valor flotante devuelto por la función
 * @param[in] p Puntero a Promesa
 */
float Promesa_ValorF(Promesa *p);

#endif