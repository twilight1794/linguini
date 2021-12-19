/**
 * @file linguini.c
 * @brief Compilador
 */

#include <stdio.h>

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "base/diccionario.h"
#include "base/lista.h"

#include "args.h"
#include "expresiones.h"
#include "lexico.h"
#include "sintactico.h"

int main(int argc, char* argv[]){
  FILE *entrada = NULL;
  char *salida = NULL;
  bool dep;
  int bands;

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
    puts(MSG_L_AYUDA_1);
    puts(MSG_L_AYUDA_2);
    puts(MSG_L_AYUDA_3);
    puts(MSG_L_AYUDA_4);
    puts(MSG_L_AYUDA_5);
    puts(MSG_L_AYUDA_6);
    puts(MSG_L_AYUDA_7);
    puts(MSG_L_AYUDA_8);
    return 0;
  }
  
  /* Versión */
  Dicc_Obtener(args, "v", &arg);
  if (((bool) *arg) == true){
    puts(MSG_L_VERSION_1);
    puts(MSG_L_VERSION_2);
    puts(MSG_L_VERSION_3);
    puts(MSG_L_VERSION_4);
    puts(MSG_L_VERSION_5);
    return 0;
  }

  /* Depuración */
  Dicc_Obtener(args, "d", &arg);
  dep = *arg;

  /* Banderas */
  Dicc_Obtener(args, "f", &arg);
  bands = *arg;

  /* Entrada */
  Dicc_Obtener(args, "i", &arg);
  if (arg == NULL) entrada = stdin;
  else entrada = fopen((char *) arg, "r");
  if (entrada == NULL){
    Registro_Registrar(true, 0, MSG_ERROR_E_S);
    Dicc_Vaciar(args);
    Dicc_Destruir(&args);
    return 1;
  } else {
    xmlDocPtr doc;
    LDE* simbolos = LDE_Crear();
    int elexico, esintactico;

    Dicc_Obtener(args, "o", &arg);
    if (arg == NULL) salida = "-";
    else salida = (char *) arg;

    /* Creación de AST */
    doc = xmlNewDoc(BAD_CAST "1.0");

    Registro_Registrar(dep, 4, MSG_EMPEZANDO_COMPILACION);
    elexico = Ling_Lexico(entrada, &simbolos);
    fclose(entrada);
    if (elexico != LING_OK){
      Registro_Registrar(dep, 1, MSG_ERROR_LEXICO);
      return 1;
    }
    esintactico = Ling_Sintactico(&simbolos, ast, dep);
    if (esintactico != LING_OK){
      Registro_Registrar(dep, 1, MSG_ERROR_SINTACTICO);
      return 1;
    }

    xmlSaveFileEnc(salida, doc, "UTF-8");

    /* Limpiar */
    xmlFreeDoc(doc);
    xmlCleanupParser();
    Dicc_Vaciar(args);
    Dicc_Destruir(&args);
  }
  return 0;
}

