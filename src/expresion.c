#include "expresion.h"
#include <stdio.h>

int Expr_Precedencia(int a,  int b){
  size_t i;
  int ch[2];
  int val[2];
  ch[0] = a;
  ch[1] = b;
  for(i=0; i<2; i++){
    if (OPERADOR_AMBITO == ch[i]) val[i] = 7;
    else if (OPERADOR_POTENCIA == ch[i] || OPERADOR_RAIZ == ch[i])  val[i] = 6;
    else if (OPERADOR_MULTIPLICACION == ch[i] || OPERADOR_DIVISION == ch[i] || OPERADOR_MODULO == ch[i])  val[i] = 5;
    else if (OPERADOR_SUMA == ch[i] || OPERADOR_RESTA == ch[i]) val[i] = 4;
    else if (OPERADOR_MAYOR == ch[i] || OPERADOR_MENOR == ch[i] || OPERADOR_MAYOR_IGUAL == ch[i] || OPERADOR_MENOR_IGUAL == ch[i]) val[i] = 3;
    else if (OPERADOR_IGUAL == ch[i] || OPERADOR_DISTINTO == ch[i]) val[i] = 2;
    else if (OPERADOR_Y == ch[i] || OPERADOR_O == ch[i]) val[i] = 1;
    else return 2;
  };
  if (val[0]>val[1]) return 1;
  else if (val[0]==val[1]) return 0;
  else return -1;
}

int Expr_ALexico(const unsigned char *e, LDE* simbolos){
  const unsigned char *c = e;
  while (*c){
    Expr_Elem* simb = NULL;
    /* Es un espacio */
    if (*c == ' ' || *c == '\t') c++;
    /* Es un número */
    else if (isdigit((char) (*c)) || *c == '.'){
      unsigned char* num = Cadena_Crear();
      bool decimal = false;
      do {
        if (*c == '.' && decimal) return (int) EXPR_NUMERO_MALFORMADO;
        else if (*c == '.') decimal = true;
        Cadena_Anadir(&num, c);
        c++;
      } while (isdigit((char) (*c)) || *c == '.');
      simb = malloc(sizeof(Expr_Elem));
      simb->tipo = SIMB_NUMERO;
      simb->val.Cad = num;
      LDE_AnadirFin(simbolos, simb);
    }
    /* Es un operador */
    else if (strchr("+-*/%^_=~><&|()[],", (char) (*c))){
      unsigned char* op = Cadena_Crear();
      unsigned char igual = '=';
      Cadena_Anadir(&op, c);
      if ((*c == '>' || *c == '<') && c[1] == '='){
        Cadena_Anadir(&op, &igual);
        c++;
      }
      c++;
      simb = malloc(sizeof(Expr_Elem));
      simb->tipo = SIMB_OPERADOR;
      simb->val.Cad = op;
      LDE_AnadirFin(simbolos, simb);
    }
    /* Es un identificador o booleano */
    else if (isalpha((char) (*c)) || *c == '_' || *c == ':'){
      unsigned char* id = Cadena_Crear();
      while(isalnum((char) (*c)) || *c == '_' || *c == ':'){
        Cadena_Anadir(&id, c);
        c++;
      }
      simb = malloc(sizeof(Expr_Elem));
      if (!strcmp("Falso", (char *) id)){
        simb->tipo = SIMB_LOGICO;
        simb->val.Bool = false;
        free(id);
      }
      else if (!strcmp("Verdadero", (char *) id)){
        simb->tipo = SIMB_LOGICO;
        simb->val.Bool = true;
        free(id);
      }
      else {
        simb->tipo = SIMB_IDENTIFICADOR;
        simb->val.Cad = id;
      }
      LDE_AnadirFin(simbolos, simb);
    }
    /* Es una cadena de texto */
    else if (*c == '"'){
      unsigned char *cad = Cadena_Crear();
      c++;
      while (*c && (*c != '"')){
        Cadena_Anadir(&cad, c);
        c++;
      }
      c++;
      simb = malloc(sizeof(Expr_Elem));
      simb->tipo = SIMB_CADENA;
      simb->val.Cad = cad;
      LDE_AnadirFin(simbolos, simb);
    }
    /* ¿Caracter ilegal? */
    else return (int) EXPR_CARACTER_ILEGAL;
  }
  return (int) EXPR_OK;
}

int Expr_ASintactico(LDE* simbolos, LDE* expresion){
  LDE *objetos = LDE_Crear(); /** Guarda los objetos de la expresión temporalmente, en orden infijo **/
  LDE *pilaOperador = LDE_Crear();
  int ultimoObj = (int) VALOR_INDEFINIDO;
  /* Para iterar sobre símbolos y objetos */
  LDE_Nodo *ptr, *post;
  Expr_Elem* val;

  /* Generar objetos */
  do {
    Expr_Elem* obj = malloc(sizeof(Expr_Elem));
    LDE_ObtenerInicio(simbolos, &ptr);
    LDE_Nodo_Obtener(ptr, (void **) &val);
    post = ptr->posterior;
    switch (val->tipo){
    case SIMB_NUMERO:
      puts("N");
      if (strchr((char *) val->val.Cad, '.') != NULL){
        obj->tipo = (int) CONSTANTE_DECIMAL;
        obj->val.Dec = strtod((char *) val->val.Cad, NULL);
      } else {
        obj->tipo = CONSTANTE_ENTERA;
        obj->val.Ent = strtol((char *) val->val.Cad, NULL, 10);
      }
      free(val->val.Cad);
      LDE_AnadirFin(objetos, obj);
      break;
    case SIMB_OPERADOR:
      puts("Op");
      obj->tipo = (int) SIMB_OPERADOR;
      if (!strcmp("[", (char *) val->val.Cad)) obj->tipo = OPERADOR_SUBEXP_A;
      else if (!strcmp("]", (char *) val->val.Cad)) obj->tipo = (int) OPERADOR_SUBEXP_C;
      else if (!strcmp("(", (char *) val->val.Cad)) obj->tipo = (int) OPERADOR_LLAMADA_A;
      else if (!strcmp(")", (char *) val->val.Cad)) obj->tipo = (int) OPERADOR_LLAMADA_C;
      else if (!strcmp("^", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_POTENCIA;
      else if (!strcmp("_", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_RAIZ;
      else if (!strcmp("*", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_MULTIPLICACION;
      else if (!strcmp("/", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_DIVISION;
      else if (!strcmp("%", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_MODULO;
      else if (!strcmp("+", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_SUMA;
      else if (!strcmp("-", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_RESTA;
      else if (!strcmp(">", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_MAYOR;
      else if (!strcmp("<", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_MENOR;
      else if (!strcmp(">=", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_MAYOR_IGUAL;
      else if (!strcmp(">=", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_MENOR_IGUAL;
      else if (!strcmp("=", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_IGUAL;
      else if (!strcmp("~", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_DISTINTO;
      else if (!strcmp("&", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_Y;
      else if (!strcmp("|", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_O;
      /* else if (!strcmp("@", (char *) val->val.Cad)) obj->val.Subtipo = (int) OPERADOR_O_EXC; */
      else if (!strcmp(",", (char *) val->val.Cad)) obj->tipo = (int) OPERADOR_SEPARADOR;
      free(val->val.Cad);
      LDE_AnadirFin(objetos, obj);
      break;
    case SIMB_IDENTIFICADOR:
      puts("I");
      obj->tipo = (int) IDENTIFICADOR;
      obj->val.Cad = val->val.Cad;
      LDE_AnadirFin(objetos, obj);
      break;
    case SIMB_LOGICO:
      puts("L");
      obj->tipo = (int) CONSTANTE_LOGICA;
      puts("PAAaa");
      obj->val.Bool = (!strcmp((char *) val->val.Cad, "Verdadero"))?true:false;
      puts("PrintAfter");
      free(val->val.Cad);
      LDE_AnadirFin(objetos, obj);
      break;
    case SIMB_CADENA:
      puts("C");
      obj->tipo = (int) CONSTANTE_CADENA;
      obj->val.Cad = val->val.Cad;
      LDE_AnadirFin(objetos, obj);
      break;
    default:
      break;
    }
    puts("A ver");
    LDE_QuitarInicio(simbolos);
  } while (post != NULL);
  puts("He aquí");
  /* Generar lista RPN */
  LDE_Inicio(objetos);
  do {
    size_t t; /* Para algunas comparaciones de tamaño */
    /* Para iterar sobre pilaOperador */
    LDE_Nodo* ptrOp;
    Expr_Elem* valOp;

    LDE_Obtener(objetos, &ptr);
    LDE_Nodo_Obtener(ptr, (void**) &val);
    switch(val->tipo){
    case CONSTANTE_DECIMAL:
      if (ultimoObj != SIMB_OPERADOR && ultimoObj != OPERADOR_SUBEXP_A && ultimoObj != VALOR_INDEFINIDO) return (int) EXPR_EXPRESION_MALFORMADA;
      LDE_AnadirFin(expresion, val);
      ultimoObj = CONSTANTE_DECIMAL;
      break;
    case CONSTANTE_ENTERA:
      if (ultimoObj != SIMB_OPERADOR && ultimoObj != OPERADOR_SUBEXP_A && ultimoObj != VALOR_INDEFINIDO) return (int) EXPR_EXPRESION_MALFORMADA;
      LDE_AnadirFin(expresion, val);
      ultimoObj = CONSTANTE_ENTERA;
      break;
    case CONSTANTE_LOGICA:
      if (ultimoObj != SIMB_OPERADOR && ultimoObj != OPERADOR_SUBEXP_A && ultimoObj != VALOR_INDEFINIDO) return (int) EXPR_EXPRESION_MALFORMADA;
      LDE_AnadirFin(expresion, val);
      ultimoObj = CONSTANTE_LOGICA;
      break;
    case CONSTANTE_CADENA:
      if (ultimoObj != SIMB_OPERADOR && ultimoObj != OPERADOR_SUBEXP_A && ultimoObj != VALOR_INDEFINIDO) return (int) EXPR_EXPRESION_MALFORMADA;
      LDE_AnadirFin(expresion, val);
      ultimoObj = CONSTANTE_CADENA;
      break;
    case IDENTIFICADOR:
      if (ultimoObj != SIMB_OPERADOR && ultimoObj != OPERADOR_SUBEXP_A && ultimoObj != VALOR_INDEFINIDO) return (int) EXPR_EXPRESION_MALFORMADA;
      LDE_AnadirFin(expresion, val);
      ultimoObj = IDENTIFICADOR;
      break;
    case SIMB_OPERADOR:
      if (ultimoObj != CONSTANTE_ENTERA && ultimoObj != CONSTANTE_DECIMAL && ultimoObj != CONSTANTE_CADENA && ultimoObj != CONSTANTE_LOGICA && ultimoObj != IDENTIFICADOR && ultimoObj != OPERADOR_SUBEXP_C && ultimoObj == VALOR_INDEFINIDO) return (int) EXPR_EXPRESION_MALFORMADA;
      LDE_Tamano(pilaOperador, &t);
      if (t>0){
        LDE_ObtenerFin(pilaOperador, &ptrOp);
        LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
      }
      while (t>0 && (valOp->tipo != OPERADOR_SUBEXP_A) && (Expr_Precedencia(valOp->val.Subtipo, val->val.Subtipo) > -1)){
        LDE_AnadirFin(expresion, valOp);
        LDE_QuitarFin(pilaOperador);
        LDE_Tamano(pilaOperador, &t); /* Actualizar t */
        if (t>0){
          LDE_ObtenerFin(pilaOperador, &ptrOp);
          LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
        }
      }
      LDE_AnadirFin(pilaOperador, val);
      ultimoObj = SIMB_OPERADOR;
      break;
    case OPERADOR_SUBEXP_A:
      if (ultimoObj != SIMB_OPERADOR && ultimoObj != OPERADOR_SUBEXP_A && ultimoObj != VALOR_INDEFINIDO) return (int) EXPR_EXPRESION_MALFORMADA;
      LDE_AnadirFin(expresion, val);
      ultimoObj = OPERADOR_SUBEXP_A;
      break;
    case OPERADOR_SUBEXP_C:
      if (ultimoObj != CONSTANTE_ENTERA && ultimoObj != CONSTANTE_DECIMAL && ultimoObj != CONSTANTE_CADENA && ultimoObj != CONSTANTE_LOGICA && ultimoObj != IDENTIFICADOR && ultimoObj != OPERADOR_SUBEXP_C && ultimoObj == VALOR_INDEFINIDO) return (int) EXPR_EXPRESION_MALFORMADA;
      LDE_ObtenerFin(pilaOperador, &ptrOp);
      LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
      while (valOp->tipo != OPERADOR_SUBEXP_A){
        LDE_AnadirFin(expresion, valOp);
        LDE_QuitarFin(pilaOperador);
        LDE_Tamano(pilaOperador, &t);
        if (!t) return (int) EXPR_PARENTESIS_EXCEDENTE;
        LDE_ObtenerFin(pilaOperador, &ptrOp);
        LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
      }
      LDE_ObtenerFin(pilaOperador, &ptrOp);
      LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
      if (valOp->tipo == OPERADOR_SUBEXP_A) LDE_QuitarFin(pilaOperador);
      ultimoObj = OPERADOR_SUBEXP_C;
      break;
    case OPERADOR_LLAMADA_A:
      if (ultimoObj != IDENTIFICADOR) return (int) EXPR_EXPRESION_MALFORMADA;
      LDE_AnadirFin(pilaOperador, val);
      /* Convertir identificador en función y moverlo a la pila */
      LDE_ObtenerFin(expresion, &ptrOp);
      LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
      valOp->tipo = IDENTIFICADOR_FUNCION;
      valOp->params = 0;
      LDE_Fin(pilaOperador);
      LDE_AnadirAnterior(pilaOperador, valOp);
      LDE_QuitarFin(expresion);
      ultimoObj = OPERADOR_LLAMADA_A;
      break;
    case OPERADOR_LLAMADA_C:
      if (ultimoObj != CONSTANTE_ENTERA && ultimoObj != CONSTANTE_DECIMAL && ultimoObj != CONSTANTE_CADENA && ultimoObj != CONSTANTE_LOGICA && ultimoObj != IDENTIFICADOR && ultimoObj != OPERADOR_LLAMADA_A && ultimoObj != OPERADOR_LLAMADA_C && ultimoObj != OPERADOR_SUBEXP_C) return (int) EXPR_EXPRESION_MALFORMADA;
      LDE_ObtenerFin(pilaOperador, &ptrOp);
      LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
      while (valOp->tipo != OPERADOR_LLAMADA_A){
        LDE_AnadirFin(expresion, valOp);
        LDE_QuitarFin(pilaOperador);
        LDE_Tamano(pilaOperador, &t);
        if (!t) return (int) EXPR_PARENTESIS_EXCEDENTE;
        LDE_ObtenerFin(pilaOperador, &ptrOp);
        LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
      }
      LDE_ObtenerFin(pilaOperador, &ptrOp);
      LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
      if (valOp->tipo == OPERADOR_SUBEXP_A) LDE_QuitarFin(pilaOperador);
      /* Mover función a salida */
      LDE_ObtenerFin(pilaOperador, &ptrOp);
      LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
      LDE_Tamano(pilaOperador, &t);
      if (t>0 && valOp->tipo == IDENTIFICADOR_FUNCION){
        LDE_AnadirFin(expresion, valOp);
        LDE_QuitarFin(pilaOperador);
      }
      valOp->params++;
      LDE_Quitar(objetos);
      ultimoObj = OPERADOR_LLAMADA_C;
      break;
    case OPERADOR_SEPARADOR:
      if (ultimoObj != CONSTANTE_ENTERA && ultimoObj != CONSTANTE_DECIMAL && ultimoObj != CONSTANTE_CADENA && ultimoObj != CONSTANTE_LOGICA && ultimoObj != IDENTIFICADOR && ultimoObj != OPERADOR_SUBEXP_C) return (int) EXPR_EXPRESION_MALFORMADA;
      LDE_ObtenerFin(pilaOperador, &ptrOp);
      LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
      while (valOp->tipo != OPERADOR_LLAMADA_A){
        LDE_AnadirFin(expresion, valOp);
        LDE_QuitarFin(pilaOperador);
        LDE_Tamano(pilaOperador, &t);
        if (!t) return (int) EXPR_PARENTESIS_EXCEDENTE;
        LDE_ObtenerFin(pilaOperador, &ptrOp);
        LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
      }
      /* Actualizar número de parámetros */
      LDE_Retroceder(pilaOperador); /* Apuntaba a una apertura de llamada, donde atrás hay una función */
      LDE_Obtener(pilaOperador, &ptrOp);
      LDE_Nodo_Obtener(ptrOp, (void**) &valOp);
      valOp->params++;
      ultimoObj = OPERADOR_SEPARADOR;
      break;
    default:
      break;
    }
    LDE_Avanzar(objetos);
  } while (ptr->posterior != NULL);
  return (int) EXPR_OK;
}
