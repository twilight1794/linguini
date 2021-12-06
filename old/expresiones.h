// Calculador de expresiones de Linguini

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include "utilidades.h"
#include "mensajes-es.h"

// Identificadores de estructuras
typedef enum {
              //// Inicialización
              VALOR_INDEFINIDO = 0,
              //// Análisis léxico
              EXP_NUMERO, EXP_IDENTIFICADOR, EXP_OPERADOR, EXP_CADENA, EXP_LOGICO, EXP_PARENTESISA, EXP_PARENTESISC,
              //// Análisis sintáctico
              // Tipos básicos=8
              VALOR_NUMERO, VALOR_CADENA, VALOR_LOGICO,
              EXPRESION_VARIABLE, EXPRESION_OPERADOR, EXPRESION_LLAMADA,
              // Operadores=14
              OPERADOR_SUMA, OPERADOR_RESTA, OPERADOR_MULTIPLICACION, OPERADOR_DIVISION, OPERADOR_MODULO, OPERADOR_POTENCIA, OPERADOR_RAIZ, OPERADOR_IGUAL, OPERADOR_DISTINTO, OPERADOR_MAYOR, OPERADOR_MENOR, OPERADOR_MAYOR_IGUAL, OPERADOR_MENOR_IGUAL, OPERADOR_Y, OPERADOR_O,
              // Funciones integradas=29
              FUNCION_ABSOLUTO, FUNCION_TRUNCAR, FUNCION_REDONDEAR, FUNCION_SIN, FUNCION_COS, FUNCION_TAN, FUNCION_ARCSIN, FUNCION_ARCCOS, FUNCION_ARCTAN, FUNCION_SINH, FUNCION_COSH, FUNCION_TANH, FUNCION_ASINH, FUNCION_ACOSH, FUNCION_ATANH, FUNCION_LOG, FUNCION_LN, FUNCION_EXP, FUNCION_FACTORIAL, FUNCION_ALEATORIO, FUNCION_MAYUSCULA, FUNCION_MINUSCULA, FUNCION_ESALNUM, FUNCION_ESNUMERO, FUNCION_ESLETRA, FUNCION_ESMAYUSCULA, FUNCION_ESMINUSCULA, FUNCION_TEXTOANUM, FUNCION_NUMATEXTO, FUNCION_LOGICOATEXTO, FUNCION_ASCII // 59
} ling_codigotipos;

// Elementos reservados
char *ling_pReservadas[16] = {"Numero", "Cadena", "Logico", "Verdadero", "Falso", "Programa", "Fin", "Si", "SiNo", "Mientras", "AsignarA", "Leer", "Escribir", "Continuar", "Regresar", "Salir"};
char *ling_funciones[31] = {"absoluto", "truncar", "redondear", "sin", "cos", "tan", "arcsin", "arccos", "arctan", "sinh", "cosh", "tanh", "asinh", "acosh", "atanh", "log", "ln", "exp", "factorial", "aleatorio", "mayuscula", "minuscula", "esalnum", "esnumero", "esletra", "esmayuscula", "esminuscula", "textoanum", "numatexto", "logicoatexto", "ascii"};

// Estructura para datos
typedef struct {
  ling_codigotipos tipo;
  union {
    char *Cad;
    double Num;
    bool Bool;
    char *nombre; // variable
    ling_codigotipos clase; // Llamada, operador
  };
} cjtExpresion;

// Funciones auxiliares

/*
 * Devuelve el nombre del tipo de dato usando su identificador
 * @param v Tipo de valor
 */
char *cadenaTipo(ling_codigotipos v){
  switch (v){
  case VALOR_NUMERO:
    return "Numero";
  case VALOR_CADENA:
    return "Cadena";
  case VALOR_LOGICO:
    return "Lógico";
  case OPERADOR_SUMA:
    return "+";
  case OPERADOR_RESTA:
    return "-";
  case OPERADOR_MULTIPLICACION:
    return "*";
  case OPERADOR_DIVISION:
    return "/";
  case OPERADOR_MODULO:
    return "%";
  case OPERADOR_POTENCIA:
    return "^";
  case OPERADOR_RAIZ:
    return "_";
  case OPERADOR_IGUAL:
    return "=";
  case OPERADOR_DISTINTO:
    return "~";
  case OPERADOR_MAYOR:
    return ">";
  case OPERADOR_MENOR:
    return "<";
  case OPERADOR_MAYOR_IGUAL:
    return ">=";
  case OPERADOR_MENOR_IGUAL:
    return "<=";
  case OPERADOR_Y:
    return "&";
  case OPERADOR_O:
    return "|";
  case FUNCION_ABSOLUTO:
    return "absoluto";
  case FUNCION_TRUNCAR:
    return "truncar";
  case FUNCION_REDONDEAR:
    return "redondear";
  case FUNCION_SIN:
    return "sin";
  case FUNCION_COS:
    return "cos";
  case FUNCION_TAN:
    return "tan";
  case FUNCION_ARCSIN:
    return "arcsin";
  case FUNCION_ARCCOS:
    return "arccos";
  case FUNCION_ARCTAN:
    return "arctan";
  case FUNCION_SINH:
    return "sinh";
  case FUNCION_COSH:
    return "cosh";
  case FUNCION_TANH:
    return "tanh";
  case FUNCION_ASINH:
    return "asinh";
  case FUNCION_ACOSH:
    return "acosh";
  case FUNCION_ATANH:
    return "atanh";
  case FUNCION_LOG:
    return "log";
  case FUNCION_LN:
    return "ln";
  case FUNCION_EXP:
    return "exp";
  case FUNCION_FACTORIAL:
    return "fact";
  case FUNCION_ALEATORIO:
    return "aleatorio";
  case FUNCION_MAYUSCULA:
    return "mayuscula";
  case FUNCION_MINUSCULA:
    return "minuscula";
  case FUNCION_ESALNUM:
    return "esalnum";
  case FUNCION_ESNUMERO:
    return "esnumero";
  case FUNCION_ESLETRA:
    return "esletra";
  case FUNCION_ESMAYUSCULA:
    return "esmayuscula";
  case FUNCION_ESMINUSCULA:
    return "esminuscula";
  case FUNCION_TEXTOANUM:
    return "textoanum";
  case FUNCION_NUMATEXTO:
    return "numatexto";
  case FUNCION_LOGICOATEXTO:
    return "logicoatexto";
  case FUNCION_ASCII:
    return "ascii";
  }
  return "x";
}

/*
 * Calcula el factorial de un número
 * @param n Operando
 */
double fact(double n){
	if (n==1) return 1;
	double f = fact(n-1)*n;
	return f;
}
/*
 * Indica si un operador tiene mayor, menor o igual precedencia que otro.
 * Devuelve 1 si a > b, 0 si son iguales, y -1 si a < b
 * @param a Identificador de un operador
 * @param b Identificador de un operador
 */
int calcPrecedencia(ling_codigotipos a,  ling_codigotipos b){
  int i;
  ling_codigotipos ch[2] = {a, b};
  int val[2];
  for(i=0; i<2; i++){
    if (OPERADOR_POTENCIA == ch[i] || OPERADOR_RAIZ == ch[i])  val[i] = 6;
    else if (OPERADOR_MULTIPLICACION == ch[i] || OPERADOR_DIVISION == ch[i] || OPERADOR_MODULO == ch[i])  val[i] = 5;
    else if (OPERADOR_SUMA == ch[i] || OPERADOR_RESTA == ch[i]) val[i] = 4;
    else if (OPERADOR_MAYOR == ch[i] || OPERADOR_MENOR == ch[i] || OPERADOR_MAYOR_IGUAL == ch[i] || OPERADOR_MENOR_IGUAL == ch[i]) val[i] = 3;
    else if (OPERADOR_IGUAL == ch[i] || OPERADOR_DISTINTO == ch[i]) val[i] = 2;
    else if (OPERADOR_Y == ch[i] || OPERADOR_O == ch[i]) val[i] = 1;
  };
  if (val[0]>val[1]) return 1;
  else if (val[0]==val[1]) return 0;
  else return -1;
}

/*
 * Imprime en la salida estándar la representación gráfica de una expresión.
 * Se usa solo para propósitos de depuración
 * @param a Objeto Array que contiene la 
*/
void imprimirArray(Array *a){
  int i;
  putchar('[');
  for(i=0;i<a->tamano;i++){
    cjtExpresion *r = (cjtExpresion*) a->valores[i];
    if (r->tipo == VALOR_CADENA) printf("(%d <Cad>) \"%s\", ", i, r->Cad);
    else if (r->tipo == VALOR_NUMERO) printf("(%d <Num>) %f, ", i, r->Num);
    else if (r->tipo == VALOR_LOGICO) printf("(%d <Bool>) %s, ", i, r->Bool?"Verdadero":"Falso");
    else if (r->tipo == EXP_PARENTESISA) printf("(%d <(>), ", i);
    else {
      if (r->tipo == EXPRESION_LLAMADA || r->tipo == EXPRESION_OPERADOR) printf("(%d <%s>), ", i, cadenaTipo(r->clase));
      else printf("(%d <%d>), ", i, r->tipo);
    }
  }
  puts("]");
}

/* Analiza una expresión y genera los símbolos de la expresión
 * @param e Cadena de texto que contiene una expresión en formato infijo
 * @param simbolos Objeto Array que va a almacenar los símbolos relevantes
 */
ling_codigomsg alExpresion(const char *e, Array *simbolos){
  const char *c = e;
  while (*c){
    // Es un espacio
    if (*c == ' ' || *c == '\t') c++;
    // Es un número
    else if (isdigit(*c) || *c == '.'){
      char *num = iniciarCadena();
      bool decimal = false;
      anadirCadena(&num, *c);
      if (*c == '.') decimal = true;
      while(*c){
        c++;
        if (*c == '.' && decimal) return Registrar(COD_EXPRESION_MALFORMADA);
        else if (*c == '.') decimal = true;
        else if (!isdigit(*c)) break;
        anadirCadena(&num, *c);
      };
      anadirArray(simbolos, num, EXP_NUMERO);
    }
    // Es un operador
    else if (strchr("+-*/%^_=~><&|", *c)){
      char *op = iniciarCadena();
      anadirCadena(&op, *c);
      if ((*c == '>' || *c == '<') && c[1] == '=') anadirCadena(&op, '=');
      anadirArray(simbolos, op, EXP_OPERADOR);
      if (!strcmp(op, ">=") || !strcmp(op, "<=")) c++;
      c++;
    }
    // Es una variable o función
    else if (isalpha(*c) || *c == '_'){
      char *id = iniciarCadena();
      //anadirCadena(&id, *c);
      while(isalnum(*c) || *c == '_'){
        anadirCadena(&id, *c);
        c++;
      }
      anadirArray(simbolos, id, (!strcmp("Falso", id) || !strcmp("Verdadero", id))?EXP_LOGICO:EXP_IDENTIFICADOR);
    }
    // Es una cadena
    else if (*c == '"'){
      char *cad = iniciarCadena();
      c++;
      while (*c && (*c != '"')){
        anadirCadena(&cad, *c);
        c++;
      }
      c++;
      anadirArray(simbolos, cad, EXP_CADENA);
    }
    // Es un paréntesis de apertura
    else if (*c == '('){
      anadirArray(simbolos, "(", EXP_PARENTESISA);
      c++;
    }
    // Es un paréntesis de cierre
    else if (*c == ')'){
      anadirArray(simbolos, ")", EXP_PARENTESISC);
      c++;
    }
    // Por ahora, tordas las funciones tienen máximo un parámetro
    // Es un separador de parámetros
    //else if (*c == ','){
    //  anadirArray(simbolos, ",", EXP_SEPARADOR);
    //  c++;
    //}
    // Caracter ilegal?
    else return Registrar(COD_CARACTER_ILEGAL, *c);
  }
  return COD_OK;
}

/*
 * Básicamente, convierte los símbolos de la expresión a una expresión RPN.
 * @param simbolos Símbolos de la expresión generados por alExpresion
 * @param expresión Objeto Array que contiene objetos de expresiones en notación polaca inversa
 */
ling_codigomsg asiExpresion(Array *simbolos, Array *expresion){
  int i;
  Array expresionI/*ntermedia*/ = iniciarArray();
  // Construir objetos
  for (i=0; i<simbolos->tamano;i++){
    cjtExpresion *expr = malloc(sizeof(cjtExpresion));
    switch(simbolos->tipos[i]){
    case EXP_NUMERO:;
      expr->tipo = VALOR_NUMERO;
      expr->Num = strtod(simbolos->valores[i], NULL);
      anadirArray(&expresionI, expr, 0);
      break;
    case EXP_IDENTIFICADOR:;
      // Función
      expr->tipo = EXPRESION_LLAMADA;
      if (!strcmp("absoluto", simbolos->valores[i])) expr->clase = FUNCION_ABSOLUTO;
      else if (!strcmp("truncar", simbolos->valores[i])) expr->clase = FUNCION_TRUNCAR;
      else if (!strcmp("redondear", simbolos->valores[i])) expr->clase = FUNCION_REDONDEAR;
      else if (!strcmp("sin", simbolos->valores[i])) expr->clase = FUNCION_SIN;
      else if (!strcmp("cos", simbolos->valores[i])) expr->clase = FUNCION_COS;
      else if (!strcmp("tan", simbolos->valores[i])) expr->clase = FUNCION_TAN;
      else if (!strcmp("arcsin", simbolos->valores[i])) expr->clase = FUNCION_ARCSIN;
      else if (!strcmp("arccos", simbolos->valores[i])) expr->clase = FUNCION_ARCCOS;
      else if (!strcmp("arctan", simbolos->valores[i])) expr->clase = FUNCION_ARCTAN;
      else if (!strcmp("sinh", simbolos->valores[i])) expr->clase = FUNCION_SINH;
      else if (!strcmp("cosh", simbolos->valores[i])) expr->clase = FUNCION_COSH;
      else if (!strcmp("tanh", simbolos->valores[i])) expr->clase = FUNCION_TANH;
      else if (!strcmp("asinh", simbolos->valores[i])) expr->clase = FUNCION_ASINH;
      else if (!strcmp("acosh", simbolos->valores[i])) expr->clase = FUNCION_ACOSH;
      else if (!strcmp("atanh", simbolos->valores[i])) expr->clase = FUNCION_ATANH;
      else if (!strcmp("log", simbolos->valores[i])) expr->clase = FUNCION_LOG;
      else if (!strcmp("ln", simbolos->valores[i])) expr->clase = FUNCION_LN;
      else if (!strcmp("exp", simbolos->valores[i])) expr->clase = FUNCION_EXP;
      else if (!strcmp("factorial", simbolos->valores[i])) expr->clase = FUNCION_FACTORIAL;
      else if (!strcmp("aleatorio", simbolos->valores[i])) expr->clase = FUNCION_ALEATORIO;
      else if (!strcmp("mayuscula", simbolos->valores[i])) expr->clase = FUNCION_MAYUSCULA;
      else if (!strcmp("minuscula", simbolos->valores[i])) expr->clase = FUNCION_MINUSCULA;
      else if (!strcmp("esalnum", simbolos->valores[i])) expr->clase = FUNCION_ESALNUM;
      else if (!strcmp("esnumero", simbolos->valores[i])) expr->clase = FUNCION_ESNUMERO;
      else if (!strcmp("esletra", simbolos->valores[i])) expr->clase = FUNCION_ESLETRA;
      else if (!strcmp("esmayuscula", simbolos->valores[i])) expr->clase = FUNCION_ESMAYUSCULA;
      else if (!strcmp("esminuscula", simbolos->valores[i])) expr->clase = FUNCION_ESMINUSCULA;
      else if (!strcmp("textoanum", simbolos->valores[i])) expr->clase = FUNCION_TEXTOANUM;
      else if (!strcmp("numatexto", simbolos->valores[i])) expr->clase = FUNCION_NUMATEXTO;
      else if (!strcmp("logicoatexto", simbolos->valores[i])) expr->clase = FUNCION_LOGICOATEXTO;
      else if (!strcmp("ascii", simbolos->valores[i])) expr->clase = FUNCION_ASCII;
      // Variable
      else {
        expr->tipo = EXPRESION_VARIABLE;
        expr->nombre = simbolos->valores[i];
      }
      anadirArray(&expresionI, expr, 0);
      break;
    case EXP_OPERADOR:;
      if (!strcmp("^", simbolos->valores[i])) expr->clase = OPERADOR_POTENCIA;
      else if (!strcmp("_", simbolos->valores[i])) expr->clase = OPERADOR_RAIZ;
      else if (!strcmp("*", simbolos->valores[i])) expr->clase = OPERADOR_MULTIPLICACION;
      else if (!strcmp("/", simbolos->valores[i])) expr->clase = OPERADOR_DIVISION;
      else if (!strcmp("%", simbolos->valores[i])) expr->clase = OPERADOR_MODULO;
      else if (!strcmp("+", simbolos->valores[i])) expr->clase = OPERADOR_SUMA;
      else if (!strcmp("-", simbolos->valores[i])) expr->clase = OPERADOR_RESTA;
      else if (!strcmp(">", simbolos->valores[i])) expr->clase = OPERADOR_MAYOR;
      else if (!strcmp("<", simbolos->valores[i])) expr->clase = OPERADOR_MENOR;
      else if (!strcmp(">=", simbolos->valores[i])) expr->clase = OPERADOR_MAYOR_IGUAL;
      else if (!strcmp("<=", simbolos->valores[i])) expr->clase = OPERADOR_MENOR_IGUAL;
      else if (!strcmp("=", simbolos->valores[i])) expr->clase = OPERADOR_IGUAL;
      else if (!strcmp("~", simbolos->valores[i])) expr->clase = OPERADOR_DISTINTO;
      else if (!strcmp("&", simbolos->valores[i])) expr->clase = OPERADOR_Y;
      else if (!strcmp("|", simbolos->valores[i])) expr->clase = OPERADOR_O;
      expr->tipo = EXPRESION_OPERADOR;
      anadirArray(&expresionI, expr, 0);
      break;
    case EXP_CADENA:;
      expr->tipo = VALOR_CADENA;
      expr->Cad = simbolos->valores[i];
      anadirArray(&expresionI, expr, 0);
      break;
    case EXP_LOGICO:;
      expr->tipo = VALOR_LOGICO;
      expr->Bool = (!strcmp(simbolos->valores[i], "Verdadero"))?true:false;
      anadirArray(&expresionI, expr, 0);
      break;
    case EXP_PARENTESISA:;
      expr->tipo = EXP_PARENTESISA;
      anadirArray(&expresionI, expr, 0);
      break;
    case EXP_PARENTESISC:;
      expr->tipo = EXP_PARENTESISC;
      anadirArray(&expresionI, expr, 0);
      break;
    }
  }
  
  // Ordenarlos en RPN
  Array pOperador = iniciarArray();
  ling_codigotipos ultimoSmb = VALOR_INDEFINIDO;
  for (i=0; i<expresionI.tamano;i++){
    cjtExpresion *expr = (cjtExpresion*) expresionI.valores[i];
    cjtExpresion *op;
    switch (expr->tipo){
    case VALOR_NUMERO:;
      if (ultimoSmb != EXPRESION_OPERADOR && ultimoSmb != EXP_PARENTESISA && ultimoSmb != VALOR_INDEFINIDO) return Registrar(COD_EXPRESION_MALFORMADA);
      anadirArray(expresion, expr, 0);
      ultimoSmb = VALOR_NUMERO;
      break;
    case VALOR_CADENA:;
      if (ultimoSmb != EXPRESION_OPERADOR && ultimoSmb != EXP_PARENTESISA && ultimoSmb != VALOR_INDEFINIDO) return Registrar(COD_EXPRESION_MALFORMADA);
      anadirArray(expresion, expr, 0);
      ultimoSmb = VALOR_CADENA;
      break;
    case VALOR_LOGICO:;
      if (ultimoSmb != EXPRESION_OPERADOR && ultimoSmb != EXP_PARENTESISA && ultimoSmb != VALOR_INDEFINIDO) return Registrar(COD_EXPRESION_MALFORMADA);
      anadirArray(expresion, expr, 0);
      ultimoSmb = VALOR_LOGICO;
      break;
    case EXPRESION_VARIABLE:;
      if (ultimoSmb != EXPRESION_OPERADOR && ultimoSmb != EXP_PARENTESISA && ultimoSmb != VALOR_INDEFINIDO) return Registrar(COD_EXPRESION_MALFORMADA);
      anadirArray(expresion, expr, 0);
      ultimoSmb = EXPRESION_VARIABLE;
      break;
    case EXPRESION_LLAMADA:;
      if (ultimoSmb != EXPRESION_OPERADOR && ultimoSmb != EXP_PARENTESISA && ultimoSmb != VALOR_INDEFINIDO) return Registrar(COD_EXPRESION_MALFORMADA);
      anadirArray(&pOperador, expr, 0);
      ultimoSmb = EXPRESION_LLAMADA;
      break;
    case EXPRESION_OPERADOR:;
      if (!i && ultimoSmb != VALOR_NUMERO && ultimoSmb != VALOR_CADENA && ultimoSmb != VALOR_LOGICO && ultimoSmb != EXPRESION_VARIABLE && ultimoSmb != EXPRESION_LLAMADA && ultimoSmb != EXP_PARENTESISC) return Registrar(COD_EXPRESION_MALFORMADA);
      if (pOperador.tamano>0) op = (cjtExpresion*) pOperador.valores[pOperador.tamano-1];
      while ((pOperador.tamano>0) && (op->tipo != EXP_PARENTESISA) && (calcPrecedencia(op->clase, expr->clase) > -1)){
        anadirArray(expresion, op, 0);
        quitarArray(&pOperador);
        if (pOperador.tamano>0) op = (cjtExpresion*) pOperador.valores[pOperador.tamano-1];
      }
      anadirArray(&pOperador, expr, 0);
      ultimoSmb = EXPRESION_OPERADOR;
      break;
    case EXP_PARENTESISA:;
      if (ultimoSmb != EXPRESION_OPERADOR && ultimoSmb != EXP_PARENTESISA && ultimoSmb != EXPRESION_LLAMADA && ultimoSmb != VALOR_INDEFINIDO) return Registrar(COD_EXPRESION_MALFORMADA);
      anadirArray(&pOperador, expr, 0);
      ultimoSmb = EXP_PARENTESISA;
      break;
    case EXP_PARENTESISC:;
      if (!i && ultimoSmb != VALOR_NUMERO && ultimoSmb != VALOR_CADENA && ultimoSmb != VALOR_LOGICO && ultimoSmb != EXPRESION_VARIABLE && ultimoSmb != EXP_PARENTESISA && ultimoSmb != EXP_PARENTESISC) return Registrar(COD_EXPRESION_MALFORMADA);
      op = (cjtExpresion*) pOperador.valores[pOperador.tamano-1];
      while(op->tipo != EXP_PARENTESISA){
        anadirArray(expresion, op, 0);
        quitarArray(&pOperador);
        if (!(pOperador.tamano)) return Registrar(COD_PARENTESIS_EXCEDENTE);
        op = (cjtExpresion*) pOperador.valores[pOperador.tamano-1];
      }
      op = (cjtExpresion*) pOperador.valores[pOperador.tamano-1];
      if (op->tipo == EXP_PARENTESISA) quitarArray(&pOperador);
      op = (cjtExpresion*) pOperador.valores[pOperador.tamano-1];
      if (pOperador.tamano > 0 && op->tipo == EXPRESION_LLAMADA){
        anadirArray(expresion, op, 0);
        quitarArray(&pOperador);
      }
      ultimoSmb = EXP_PARENTESISC;
      break;
    }
  }
  while (pOperador.tamano>0){
    cjtExpresion *op = (cjtExpresion*) pOperador.valores[pOperador.tamano-1];
    if ((op->tipo == EXP_PARENTESISA) || (op->tipo == EXP_PARENTESISC)) return Registrar(COD_PARENTESIS_EXCEDENTE);
    anadirArray(expresion, op, 0);
    quitarArray(&pOperador);
  }
  return COD_OK;
}

/*
 * Evalua una expresión
 * @param expresion: objeto Array que contiene una expresión creada por la función asiExpresion
 * @param valor: valor al que se escribirá el resultado de la evaluación de la expresión
 * @param variables: objeto Array que contiene las variables asignadas hasta ese momento
 */
ling_codigomsg aseExpresion(Array *expresion, cjtExpresion **valor, Dict *variables){
  int i;
  Array pila = iniciarArray();
  for (i=0; i<expresion->tamano; i++){
    //imprimirArray(expresion);
    //imprimirArray(&pila);
    cjtExpresion *expr = (cjtExpresion*) expresion->valores[i];
    cjtExpresion *temp; // Puntero temporal para valores creados
    bool es = true; // Para FUNCION
    size_t j; // Iterador genérico
    cjtExpresion *op1, *op2 = NULL; // Punteros a los operandos a evaluar
    switch (expr->tipo){
    case VALOR_NUMERO:;
      anadirArray(&pila, expr, 0);
      break;
    case VALOR_CADENA:;
      anadirArray(&pila, expr, 0);
      break;
    case VALOR_LOGICO:;
      anadirArray(&pila, expr, 0);
      break;
    case EXPRESION_VARIABLE:;
      int pos = enArray(expr->nombre, &variables->identificadores);
      if (pos == -1) return Registrar(COD_VARIABLE_INDEFINIDA, expr->nombre);
      else anadirArray(&pila, variables->valores.valores[pos], 0);
      break;
    case EXPRESION_OPERADOR:;
      // Todos los operadores son binarios
      if (pila.tamano < 2) return Registrar(COD_EXPRESION_MALFORMADA);
      // Obtener tipos
      op1 = (cjtExpresion*) pila.valores[pila.tamano-2];
      op2 = (cjtExpresion*) pila.valores[pila.tamano-1];
      // Si dos valores no son del mismo tipo
      if (op1->tipo != op2->tipo) return Registrar(COD_TIPO_INCOMPATIBLE, cadenaTipo(op2->tipo), cadenaTipo(op1->tipo));
      switch (expr->clase){
      case OPERADOR_SUMA:;
        if (op1->tipo == VALOR_NUMERO){
          temp = malloc(sizeof(cjtExpresion));
          temp->Num = op1->Num + op2->Num;
          temp->tipo = VALOR_NUMERO;
          quitarArray(&pila);
          quitarArray(&pila);
          anadirArray(&pila, temp, 0);
        }
        else if (op1->tipo == VALOR_CADENA){
          temp = malloc(sizeof(cjtExpresion));
          size_t t = strlen(op1->Cad) + strlen(op2->Cad) + 1;
          temp->Cad = calloc(t, 1);
          temp->tipo = VALOR_CADENA;
          strncat(temp->Cad, op1->Cad, t);
          strncat(temp->Cad, op2->Cad, t);
          quitarArray(&pila);
          quitarArray(&pila);
          anadirArray(&pila, temp, 0);
        }
        else return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero o Cadena", cadenaTipo(expr->clase));
        break;
      case OPERADOR_RESTA:;
        if (op1->tipo == VALOR_NUMERO){
          temp = malloc(sizeof(cjtExpresion));
          temp->Num = op1->Num - op2->Num;
          temp->tipo = VALOR_NUMERO;
          quitarArray(&pila);
          quitarArray(&pila);
          anadirArray(&pila, temp, 0);
        }
        else return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        break;
      case OPERADOR_MULTIPLICACION:;
        if (op1->tipo == VALOR_NUMERO){
          temp = malloc(sizeof(cjtExpresion));
          temp->Num = op1->Num * op2->Num;
          temp->tipo = VALOR_NUMERO;
          quitarArray(&pila);
          quitarArray(&pila);
          anadirArray(&pila, temp, 0);
        }
        else return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        break;
      case OPERADOR_DIVISION:;
        if (op1->tipo == VALOR_NUMERO){
          temp = malloc(sizeof(cjtExpresion));
          temp->Num = op1->Num / op2->Num;
          temp->tipo = VALOR_NUMERO;
          quitarArray(&pila);
          quitarArray(&pila);
          anadirArray(&pila, temp, 0);
        }
        else return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        break;
      case OPERADOR_MODULO:;
        if (op1->tipo == VALOR_NUMERO){
          temp = malloc(sizeof(cjtExpresion));
          temp->Num = (int) op1->Num % (int) op2->Num;
          temp->tipo = VALOR_NUMERO;
          quitarArray(&pila);
          quitarArray(&pila);
          anadirArray(&pila, temp, 0);
        }
        else return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        break;
      case OPERADOR_POTENCIA:;
        if (op1->tipo == VALOR_NUMERO){
          temp = malloc(sizeof(cjtExpresion));
          temp->Num = pow(op1->Num, op2->Num);
          temp->tipo = VALOR_NUMERO;
          quitarArray(&pila);
          quitarArray(&pila);
          anadirArray(&pila, temp, 0);
        }
        else return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        break;
      case OPERADOR_RAIZ:;
        if (op1->tipo == VALOR_NUMERO){
          temp = malloc(sizeof(cjtExpresion));
          temp->Num = exp((1/op2->Num)*log(op1->Num));
          temp->tipo = VALOR_NUMERO;
          quitarArray(&pila);
          quitarArray(&pila);
          anadirArray(&pila, temp, 0);
        }
        else return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        break;
      case OPERADOR_IGUAL:;
        temp = malloc(sizeof(cjtExpresion));
        if (op1->tipo == VALOR_NUMERO) temp->Bool = (bool) (op1->Num == op2->Num);
        else if (op1->tipo == VALOR_CADENA) temp->Bool = (bool) !strcmp(op1->Cad, op2->Cad);
        else if (op1->tipo == VALOR_LOGICO) temp->Bool = (bool) (op1->Bool == op2->Bool);
        temp->tipo = VALOR_LOGICO;
        quitarArray(&pila);
        quitarArray(&pila);
        anadirArray(&pila, temp, VALOR_LOGICO);
        break;
      case OPERADOR_DISTINTO:;
        temp = malloc(sizeof(cjtExpresion));
        if (op1->tipo == VALOR_NUMERO) temp->Bool = (bool) (op1->Num != op2->Num);
        else if (op1->tipo == VALOR_CADENA) temp->Bool = (bool) strcmp(op1->Cad, op2->Cad);
        else if (op1->tipo == VALOR_LOGICO) temp->Bool = (bool) (op1->Bool != op2->Bool);
        temp->tipo = VALOR_LOGICO;
        quitarArray(&pila);
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case OPERADOR_MAYOR:;
        if (op1->tipo == VALOR_NUMERO){
          temp = malloc(sizeof(cjtExpresion));
          temp->Bool = (bool) (op1->Num > op2->Num);
          temp->tipo = VALOR_LOGICO;
          quitarArray(&pila);
          quitarArray(&pila);
          anadirArray(&pila, temp, 0);
        }
        else return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        break;
      case OPERADOR_MENOR:;
        if (op1->tipo == VALOR_NUMERO){
          temp = malloc(sizeof(cjtExpresion));
          temp->Bool = (bool) (op1->Num < op2->Num);
          temp->tipo = VALOR_LOGICO;
          quitarArray(&pila);
          quitarArray(&pila);
          anadirArray(&pila, temp, 0);
        }
        else return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        break;
      case OPERADOR_MAYOR_IGUAL:;
        if (op1->tipo == VALOR_NUMERO){
          temp = malloc(sizeof(cjtExpresion));
          temp->Bool = (bool) (op1->Num >= op2->Num);
          temp->tipo = VALOR_LOGICO;
          quitarArray(&pila);
          quitarArray(&pila);
          anadirArray(&pila, temp, 0);
        }
        else return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        break;
      case OPERADOR_MENOR_IGUAL:;
        if (op1->tipo == VALOR_NUMERO){
          temp = malloc(sizeof(cjtExpresion));
          temp->Bool = (bool) (op1->Num <= op2->Num);
          temp->tipo = VALOR_LOGICO;
          quitarArray(&pila);
          quitarArray(&pila);
          anadirArray(&pila, temp, 0);
        }
        else return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        break;
      case OPERADOR_Y:;
        temp = malloc(sizeof(cjtExpresion));
        if (op1->tipo == VALOR_NUMERO) temp->Bool = (bool) (op1->Num && op2->Num);
        else if (op1->tipo == VALOR_CADENA) temp->Bool = (bool) strlen(op1->Cad) && (bool) strlen(op2->Cad);
        else if (op1->tipo == VALOR_LOGICO) temp->Bool = (bool) (op1->Bool && op2->Bool);
        temp->tipo = VALOR_LOGICO;
        quitarArray(&pila);
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case OPERADOR_O:;
        temp = malloc(sizeof(cjtExpresion));
        if (op1->tipo == VALOR_NUMERO) temp->Bool = (bool) (op1->Num ||  op2->Num);
        else if (op1->tipo == VALOR_CADENA) temp->Bool = (bool) strlen(op1->Cad) || (bool) strlen(op2->Cad);
        else if (op1->tipo == VALOR_LOGICO) temp->Bool = (bool) (op1->Bool || op2->Bool);
        temp->tipo = VALOR_LOGICO;
        quitarArray(&pila);
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      }
      break;
    case EXPRESION_LLAMADA:;
      if (pila.tamano < 1) return Registrar(COD_EXPRESION_MALFORMADA);
      else op1 = (cjtExpresion*) pila.valores[pila.tamano-1];
      switch (expr->clase){
      case FUNCION_ABSOLUTO:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = fabs(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_TRUNCAR:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = (int) op1->Num;
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_REDONDEAR:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = round(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_SIN:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = sin(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_COS:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = cos(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_TAN:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = tan(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ARCSIN:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = asin(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ARCCOS:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = acos(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ARCTAN:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = atan(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_SINH:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = sinh(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_COSH:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = cosh(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_TANH:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = tanh(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ASINH:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = asinh(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ACOSH:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = acosh(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ATANH:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = atanh(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_LOG:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = log10(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_LN:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = log(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_EXP:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = exp(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_FACTORIAL:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = fact(op1->Num);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ALEATORIO:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero",  cadenaTipo(expr->clase));
        srand((unsigned int) time(NULL));
        temp = malloc(sizeof(cjtExpresion));
        int rd = rand();
        if (op1->Num > 0) temp->Num = rd/(RAND_MAX/op1->Num);
        else temp->Num = rd;
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_MAYUSCULA:;
        if (op1->tipo != VALOR_CADENA) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Cadena",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Cad = iniciarCadena();
        for (j=0;j<strlen(op1->Cad);j++){
          anadirCadena(&temp->Cad, (char) toupper(op1->Cad[j]));
        }
        temp->tipo = VALOR_CADENA;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_MINUSCULA:
        if (op1->tipo != VALOR_CADENA) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Cadena",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        for (j=0;j<strlen(op1->Cad);j++){
          anadirCadena(&temp->Cad, (char) tolower(op1->Cad[j]));
        }
        temp->tipo = VALOR_CADENA;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ESALNUM:
        if (op1->tipo != VALOR_CADENA) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Cadena",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        for (j=0;j<strlen(op1->Cad);j++){
          if (!isalnum(op1->Cad[j])) es = false;
        }
        temp->Bool = es;
        es = true;
        temp->tipo = VALOR_LOGICO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ESNUMERO:
        if (op1->tipo != VALOR_CADENA) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Cadena",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        for (j=0;j<strlen(op1->Cad);j++){
          if (!isdigit(op1->Cad[j]) && op1->Cad[j] != '.') es = false;
        }
        temp->Bool = es;
        es = true;
        temp->tipo = VALOR_LOGICO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ESLETRA:
        if (op1->tipo != VALOR_CADENA) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Cadena",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        for (j=0;j<strlen(op1->Cad);j++){
          if (!isalpha(op1->Cad[j])) es = false;
        }
        temp->Bool = es;
        es = true;
        temp->tipo = VALOR_LOGICO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ESMAYUSCULA:
        if (op1->tipo != VALOR_CADENA) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Cadena",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        for (j=0;j<strlen(op1->Cad);j++){
          if (!isupper(op1->Cad[j])) es = false;
        }
        temp->Bool = es;
        es = true;
        temp->tipo = VALOR_LOGICO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ESMINUSCULA:
        if (op1->tipo != VALOR_CADENA) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Cadena",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        for (j=0;j<strlen(op1->Cad);j++){
          if (!islower(op1->Cad[j])) es = false;
        }
        temp->Bool = es;
        es = true;
        temp->tipo = VALOR_LOGICO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_TEXTOANUM:
        if (op1->tipo != VALOR_CADENA) Registrar(COD_OPERADOR_INCOMPATIBLE, "Cadena", cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Num = strtod(op1->Cad, NULL);
        temp->tipo = VALOR_NUMERO;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_NUMATEXTO:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Cad = malloc(1);
        int nc = (int) snprintf(temp->Cad, 1, "%f", op1->Num);
        if (nc<=0) return Registrar(COD_EXPRESION_MALFORMADA);
        temp->Cad = (char *) realloc(temp->Cad, (size_t) nc+1);
        snprintf(temp->Cad, (size_t) nc, "%f", op1->Num);
        temp->tipo = VALOR_CADENA;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_LOGICOATEXTO:
        if (op1->tipo != VALOR_LOGICO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        temp->Cad = iniciarCadena();
        if (op1->Bool) anadirSCadena(&temp->Cad, "Verdadero");
        else anadirSCadena(&temp->Cad, "Falso");
        temp->tipo = VALOR_CADENA;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
        break;
      case FUNCION_ASCII:
        if (op1->tipo != VALOR_NUMERO) return Registrar(COD_OPERADOR_INCOMPATIBLE, "Numero",  cadenaTipo(expr->clase));
        temp = malloc(sizeof(cjtExpresion));
        char car = (char) strtol(op1->Cad, NULL, 0);
        temp->Cad = iniciarCadena();
        anadirCadena(&temp->Cad, car);
        temp->tipo = VALOR_CADENA;
        quitarArray(&pila);
        anadirArray(&pila, temp, 0);
      }
      break;
    }
  }
  if (pila.tamano != 1) return COD_EXPRESION_MALFORMADA;
  *valor = (cjtExpresion*) pila.valores[0];
  return COD_OK;
}
