#include "sintactico.h"

int Ling_Sintactico(LDE* simbolos, bool dep){
  /* Preparar AST */
  xmlNodePtr cur = NULL, raiz = xmlNewNode(NULL, BAD_CAST "linguini");
  xmlNsPtr espn = xmlNewNs(raiz, BAD_CAST "https://www.raiponce.xyz/proyectos/linguini#", BAD_CAST "l");
  xmlSetNs(raiz, espn);
  xmlDocSetRootElement(doc, root_node);
  
  /*xmlNewChild(raiz, espn, BAD_CAST "PowerScheme", NULL);*/
}