/**
 * @file remy.c
 * @brief Máquina virtual de Linguini
 * @author Giovanni Alfredo Garciliano Díaz
 * @date 18 de diciembre de 2021
 * @version 2.0
 * @copyright Licencia Pública General de GNU 3.0 o posterior
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "expresiones.h"
#include "recolector.h"
#include "base/cadena.h"
#include "base/diccionario.h"
#include "base/lista.h"
#include "biblioteca/base.h"
#include "biblioteca/matematica.h"
#include "biblioteca/es.h"
#include "config.h"


enum Remy_Codigos {
  REMY_OK = 100,
  REMY_INSTRUCCION_INVALIDA,
  REMY_COD_SALIDA_SISTEMA,
  REMY_ERROR_SISTEMA
}

/*
 * Marco de pila
 */

/*
 * Elemento de marco de pila
 */
typedef struct {
  Cadena id; /**<   */
  bool principal; /**< Indica si el valor es una referencia a otro objeto */
  size_t val;
} MarcoPila_Elem;

static MarcoPila_Elem* MarcoPila_Elem_Crear(Cadena id, bool principal, void *val){
  MarcoPila_Elem* e = (MarcoPila_Elem*) malloc(sizeof(MarcoPilaElem));
  if (e != NULL){
    Cadena
  }
  return e;
}

static void MarcoPila_Elem_Crear(MarcoPila_Elem** e){
  
}

/*
 * @brief 
 * @param padre Bloque padre en el cual basar el marco de pila a crear. Si es nulo, quiere decir que el bloque es una unidad
 * @param bloque Nombre del bloque del marco de pila
 * @param obj Objeto que representa el bloque del marco de pila
 */
static Dicc* MarcoPila_Crear(Dicc *padre, Cadena bloque){
  /* Copiar elementos padre */
  Dicc* m = Dicc_Crear();
  /* Estamos en el espacio global */
  if (padre != NULL){
    size_t i = 0;
    Dicc_Anadir(m, bloque, obj);
    /* Base */
    Cadena* Cadena_Crear();
    for ()
  }
}

static int MarcoPila__(Dicc_Entrada** v){
  free(dato);
}

static int MarcoPila_Destruir(Dicc **d){
  Dicc_PorCada(*d, MarcoPila_Destruir_, REMY_OK);
  Dicc_Vaciar(*d);
  Dicc_Destruir(d);
}

/**
 * @brief Comprueba si una etiqueta tiene un nombre determinado
 * @param cur 
 * @param cad 
 */
inline static bool compararElem(const xmlNodePtr cur, char* cad){
  return xmlStrcmp(cur->name, (const xmlChar *) cad) && xmlStrcmp(cur->ns->href, (const xmlChar *) ESPN_LING);
}

/**
 * @brief Libera todas las estructuras principales usadas por el intérprete
 * @param datos Lista de objetos
 * @param marco Marco de pila global
 */
static void Remy_Finalizar(Remy_Objetos** datos, Dicc** marco){
  Recolector_Destruir(*datos);
  MarcoPila_Destruir(marco);
  return 0;
}

/*
 * Ejecuta una instrucción
 * @param instruccion Instrucción a ejecutar
 * @param tipoPadre Bloque padre de la instrucción a ejecutar
 * @param datos Lista de objetos
 * @param marco Marco de pila padre
 */
int Remy_EjecInstruccion(xmlNodePtr instruccion, xmlNodePtr padre, Remy_Objetos* datos, Dicc* marco){
  /* Sentencias */
  if (!compararElem(cur, "declarar")){
    
  } else if (!compararElem(cur, "asignar")){
    
  } else if (!compararElem(cur, "eliminar")){
    
  } else if (!compararElem(cur, "usar")){
    
  } else if (!compararElem(cur, "continuar")){
    
  } else if (!compararElem(cur, "regresar")){
    
  }
  /* Bloques */
  else if (!compararElem(cur, "funcion")){
    
  } else if (!compararElem(cur, "clase")){
    
  } else if (!compararElem(cur, "condicional")){
    
  } else if (!compararElem(cur, "porcada")){
    
  } else if (!compararElem(cur, "escoger")){
    
  } else {
    return (int) REMY_INSTRUCCION_INVALIDA;
  }
}

int main(int argc, char *argv[]){
  char *entrada;
  xmlDocPtr doc;
  xmlNodePtr cur;

  Dicc *args; /* Diccionario de argumentos */
  void *arg; /* Argumento a manejar */
  int est = Args_Procesar(argc, argv, &args);
  if (est == ARGS_PARAM_DESCONOCIDO){
    /* true, porque son errores de máxima importancia */
    Registro_Registrar(true, 0, MSG_PARAM_DESCONOCIDO);
    return 1;
  } else if (est == ARGS_PARAM_INCOMPLETO){
    Registro_Registrar(true, 0, MSG_PARAM_INCOMPLETO);
    return 1;
  } else if (est == ARGS_VAL_F_INVALIDO){
    Registro_Registrar(true, 0, MSG_VAL_F_INVALIDO);
    return 1;
  }

  /* Ayuda */
  Dicc_Obtener(args, "h", &arg);
  if (((bool) *arg) == true){
    puts(MSG_R_AYUDA_1);
    puts(MSG_R_AYUDA_2);
    puts(MSG_R_AYUDA_3);
    puts(MSG_R_AYUDA_4);
    puts(MSG_R_AYUDA_5);
    puts(MSG_R_AYUDA_6);
    return 0;
  }
  
  /* Versión */
  Dicc_Obtener(args, "v", &arg);
  if (((bool) *arg) == true){
    puts(MSG_VERSION_1);
    puts(MSG_VERSION_2);
    puts(MSG_VERSION_3);
    puts(MSG_VERSION_4);
    puts(MSG_VERSION_5);
    return 0;
  }

  /* Entrada */
  Dicc_Obtener(args, "i", &arg);
  if (arg == NULL) entrada = "-";
  doc = xmlParseFile(entrada);
  
  if (doc == NULL){
    Registro_Registrar(true, 0, MSG_ERROR_E_S_XML);
    return 1;
  }
  cur = xmlDocGetRootElement(doc);
  if (cur == NULL){
    Registro_Registrar(true, 0, MSG_ERROR_E_S_XML);
    xmlFreeDoc(doc);
    return 1;
  }
  if (compararElems(cur, "linguini")){
    Registro_Registrar(true, 0, MSG_ERROR_XML_ERRONEO);
    xmlFreeDoc(doc);
    return 1;
  } else {
    /* En este punto, estamos listos para analizar semánticamente */
    xmlChar* id = xmlGetNsProp(cur, "id", ESPN_LING);
    xmlChar* tipo = xmlGetNsProp(cur, "tipo", ESPN_LING);
    if (!xmlStrcmp(tipo, (const xmlChar *) "0")){
      Remy_Objetos* datos;
      Dicc* pila;

      Registro_Registrar(true, 4, MSG_DEP_INICIO_PROGRAMA);
      datos = Recolector_Crear();
      pila = MarcoPila_Crear(NULL); /* Marco de pila global */
      cur = cur->xmlChildrenNode;
      while (cur != NULL){
        int cod = Remy_EjecInstruccion(cur, xmlNodePtr padre, Remy_Objetos* datos, Dicc* marco);
        if (cod != REMY_OK) return cod;
        cur = cur->next;
      }
      Registro_Registrar(true, 4, MSG_DEP_FIN_PROGRAMA);
    } else {
      Registro_Registrar(true, 1, MSG_DEP_ESMODULO);
      xmlFreeDoc(doc);
      return 1;
    }
  }
}