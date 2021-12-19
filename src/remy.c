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

int main(int argc, char *argv[]){
  char *entrada;

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
  xmlDocPtr doc = xmlParseFile(entrada);
  xmlNodePtr cur;
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
  if (xmlStrcmp(cur->name, (const xmlChar *) "linguini") && xmlStrcmp(cur->ns->href, (const xmlChar *) "https://www.raiponce.xyz/proyectos/linguini#")){
    Registro_Registrar(true, 0, MSG_ERROR_XML_ERRONEO);
    xmlFreeDoc(doc);
    return 1;
  }
  /* En este punto, estamos listos para analizar semánticamente */
  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"storyinfo"))){
      parseStory (doc, cur);
    }
    cur = cur->next;
  }
}