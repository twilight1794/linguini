// Linguini 1.0.1
// GPLv3

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

//#include <unistd.h>

#include "expresiones.h"

// Identificadores de estructuras
typedef enum {
              //// Análisis léxico
              SIMBOLO_SENTENCIA = 1, SIMBOLO_EXPRESION,
              //// Análisis sintáctico
              // Conjuntos = 3
              CONJUNTO_EXPRESION, CONJUNTO_INSTRUCCION,
              // Bloques = 5
              BLOQUE_MODULO, BLOQUE_SI, BLOQUE_MIENTRAS,
              // Sentencias = 8
              SENTENCIA_DEFINIR, SENTENCIA_ASIGNARA, SENTENCIA_LEER, SENTENCIA_ESCRIBIR, SENTENCIA_CONTINUAR, SENTENCIA_REGRESAR, SENTENCIA_SALIR
} ling_codigotipo;

//Estructura para almacenar datos útiles sobre el programa
typedef struct {
  bool pasoAPaso;
  int estado;
} strDatos;
strDatos prog;

// Estructuras
typedef struct {
  ling_codigotipo tipo;
  ling_codigotipos tipodato; // definir
  char *variable; // leer, asignar, definir
  Array expresion; // salir, escribir, asignar, mientras, si
  Array instrucciones; //mientras, si
  Array instruccionesn; // si
} cjtInstruccion;

typedef struct {
  char *nombre;
  Array instrucciones;
} blqModulo;

// Intérprete
/*
 * Analizador léxico. Lee caracteres del código fuente y genera un array de símbolos.
 * @param archivo Archivo que contiene el código fuente del programa
 * @param simbolos Objeto Array con los símbolos generados
 */
ling_codigomsg alexico(FILE *archivo, Array *simbolos){
  char c;
  bool sentencia = true;

  while(!feof(archivo)){
    c = (char) fgetc(archivo);
    if (c == '#') while (c != '\n'){c = (char) fgetc(archivo);}
    else if (c == ' ' || c == '\r' || c == '\t' || c == '\n') continue;
    // Sentencia
    else if (isalpha(c) && sentencia){
      sentencia = false;
      char *tkn = iniciarCadena();
      while (isalpha(c) && c != ' ' && c != '\r' && c != '\t' && c != '\n'){
        anadirCadena(&tkn, c);
        c = (char) fgetc(archivo);
      }
      if (c != ' ' && c != '\r' && c != '\t' && c != '\n') return Registrar(COD_CARACTER_ILEGAL, c);
      if (c == '\n') sentencia = true;
      anadirArray(simbolos, tkn, SIMBOLO_SENTENCIA);
    }
    // Expresion
    else if (!sentencia && (isalnum(c) || c == '_' || c == '"')){
      char *tkn = iniciarCadena();
      while (c != '\n'){
        sentencia = true;
        anadirCadena(&tkn, c);
        c = (char) fgetc(archivo);
      }
      anadirArray(simbolos, tkn, SIMBOLO_EXPRESION);
    }
    else if (feof(archivo)) return COD_OK;
    else return Registrar(COD_CARACTER_ILEGAL, c);
  }
  return COD_OK;
}

/*
 * Analiza cada símbolo de instrucción para crear un nodo para el AST
 * @param simbolos Objeto Array que contiene los símbolos de instrucciones
 * @param i Iterador provisto por asintactico para iterar en simbolos
 * @param nodo Objeto en donde se guarda la estructura de una instrucción
 */
ling_codigomsg constInstruccion(Array *simbolos, int *i, cjtInstruccion **nodo){
  if (!strcmp(simbolos->valores[*i], "Programa")) return Registrar(COD_PROGRAMA_MULTIPLE);
  else if (!strcmp(simbolos->valores[*i], "Si")){
    *nodo = malloc(sizeof(cjtInstruccion));
    (*nodo)->tipo = BLOQUE_SI;
    // Expresion
    (*i)++;
    if (simbolos->tipos[*i] != SIMBOLO_EXPRESION) return Registrar(COD_INSTRUCCION_INCOMPLETA, simbolos->valores[(*i)-1]);
    Array expr = iniciarArray();
    ling_codigomsg err = alExpresion(simbolos->valores[*i], &expr);
    if (err) return err;
    (*nodo)->expresion = iniciarArray();
    err = asiExpresion(&expr, &(*nodo)->expresion);
    if (err) return err;
    (*i)++;
    // Condiciones
    bool si = true;
    (*nodo)->instrucciones = iniciarArray();
    (*nodo)->instruccionesn = iniciarArray();
    while (strcmp(simbolos->valores[*i], "Fin")){
      if (!strcmp(simbolos->valores[*i], "SiNo")){
        si = false;
        (*i)++;
        if (simbolos->tipos[*i] != SIMBOLO_SENTENCIA) return Registrar(COD_EXPRESION_IMPROCEDENTE);
      }
      cjtInstruccion *inst;
      err = constInstruccion(simbolos, i, &inst);
      if (err) return err;
      if (si) anadirArray(&(*nodo)->instrucciones, inst, 0);
      else anadirArray(&(*nodo)->instruccionesn, inst, 0);
      (*i)++;
    }
  }
  else if (!strcmp(simbolos->valores[*i], "Mientras")){
    *nodo = malloc(sizeof(cjtInstruccion));
    (*nodo)->tipo = BLOQUE_MIENTRAS;
    // Expresion
    (*i)++;
    if (simbolos->tipos[*i] != SIMBOLO_EXPRESION) return Registrar(COD_INSTRUCCION_INCOMPLETA, simbolos->valores[(*i)-1]);
    Array expr = iniciarArray();
    ling_codigomsg err = alExpresion(simbolos->valores[*i], &expr);
    if (err) return err;
    (*nodo)->expresion = iniciarArray();
    err = asiExpresion(&expr, &(*nodo)->expresion);
    if (err) return err;
    (*i)++;
    // Condiciones
    (*nodo)->instrucciones = iniciarArray();
    while (strcmp(simbolos->valores[*i], "Fin")){
      cjtInstruccion *inst;
      err = constInstruccion(simbolos, i, &inst);
      if (err) return err;
      anadirArray(&(*nodo)->instrucciones, inst, 0);
      (*i)++;
    }
  }
  else if (!strcmp(simbolos->valores[*i], "Fin")) return Registrar(COD_FIN_IMPROCEDENTE);
  else if (!strcmp(simbolos->valores[*i], "Numero") || !strcmp(simbolos->valores[*i], "Cadena") || !strcmp(simbolos->valores[*i], "Logico")){
    *nodo = malloc(sizeof(cjtInstruccion));
    (*nodo)->tipo = SENTENCIA_DEFINIR;
    (*i)++;
    if (simbolos->tipos[*i] != SIMBOLO_EXPRESION) return Registrar(COD_INSTRUCCION_INCOMPLETA, simbolos->valores[(*i)-1]);
    (*nodo)->variable = simbolos->valores[*i];
    if (!strcmp(simbolos->valores[(*i)-1], "Numero")) (*nodo)->tipodato = VALOR_NUMERO;
    else if (!strcmp(simbolos->valores[(*i)-1], "Cadena")) (*nodo)->tipodato = VALOR_CADENA;
    else if (!strcmp(simbolos->valores[(*i)-1], "Logico")) (*nodo)->tipodato = VALOR_LOGICO;
  }
  else if (!strcmp(simbolos->valores[*i], "AsignarA")){
    *nodo = malloc(sizeof(cjtInstruccion));
    (*nodo)->tipo = SENTENCIA_ASIGNARA;
    (*i)++;
    if (simbolos->tipos[*i] != SIMBOLO_EXPRESION) return Registrar(COD_INSTRUCCION_INCOMPLETA, simbolos->valores[(*i)-1]);
    (*nodo)->variable = iniciarCadena();
    char *c = simbolos->valores[*i];
    char *exprC = iniciarCadena();
    while (*c && *c != ' ' && *c != '\t'){
      anadirCadena(&(*nodo)->variable, *c);
      c++;
    }
    if (!(*c)) return Registrar(COD_INSTRUCCION_INCOMPLETA, simbolos->valores[(*i)-1]);
    while (*c && (*c == ' ' || *c == '\t')){c++;}
    if (!(*c)) return Registrar(COD_INSTRUCCION_INCOMPLETA, simbolos->valores[(*i)-1]);
    while (*c){
      anadirCadena(&exprC, *c);
      c++;
    }
    Array expr = iniciarArray();
    ling_codigomsg err = alExpresion(exprC, &expr);
    if (err) return err;
    (*nodo)->expresion = iniciarArray();
    err = asiExpresion(&expr, &(*nodo)->expresion);
    if (err) return err;
  }
  else if (!strcmp(simbolos->valores[*i], "Leer")){
    *nodo = malloc(sizeof(cjtInstruccion));
    (*nodo)->tipo = SENTENCIA_LEER;
    (*i)++;
    if (simbolos->tipos[*i] != SIMBOLO_EXPRESION) return Registrar(COD_INSTRUCCION_INCOMPLETA, simbolos->valores[(*i)-1]);
    (*nodo)->variable = simbolos->valores[*i];
  }
  else if (!strcmp(simbolos->valores[*i], "Escribir")){
    *nodo = malloc(sizeof(cjtInstruccion));
    (*nodo)->tipo = SENTENCIA_ESCRIBIR;
    (*i)++;
    if (simbolos->tipos[*i] != SIMBOLO_EXPRESION) return Registrar(COD_INSTRUCCION_INCOMPLETA, simbolos->valores[(*i)-1]);
    Array expr = iniciarArray();
    ling_codigomsg err = alExpresion(simbolos->valores[*i], &expr);
    if (err) return err;
    (*nodo)->expresion = iniciarArray();
    err = asiExpresion(&expr, &(*nodo)->expresion);
    if (err) return err;
  }
  else if (!strcmp(simbolos->valores[*i], "Continuar")){
    *nodo = malloc(sizeof(cjtInstruccion));
    (*nodo)->tipo = SENTENCIA_CONTINUAR;
    (*i)++;
    if (simbolos->tipos[*i] == SIMBOLO_EXPRESION) return Registrar(COD_EXPRESION_IMPROCEDENTE);
    (*i)--;
  }
  else if (!strcmp(simbolos->valores[*i], "Regresar")){
    *nodo = malloc(sizeof(cjtInstruccion));
    (*nodo)->tipo = SENTENCIA_REGRESAR;
    (*i)++;
    if (simbolos->tipos[*i] == SIMBOLO_EXPRESION) return Registrar(COD_EXPRESION_IMPROCEDENTE);
    (*i)--;
  }
  else if (!strcmp(simbolos->valores[*i], "Salir")){
    *nodo = malloc(sizeof(cjtInstruccion));
    (*nodo)->tipo = SENTENCIA_SALIR;
    (*i)++;
    if (simbolos->tipos[*i] != SIMBOLO_EXPRESION) return Registrar(COD_INSTRUCCION_INCOMPLETA, simbolos->valores[(*i)-1]);
    Array expr = iniciarArray();
    ling_codigomsg err = alExpresion(simbolos->valores[*i], &expr);
    if (err) return err;
    (*nodo)->expresion = iniciarArray();
    err = asiExpresion(&expr, &(*nodo)->expresion);
    if (err) return err;
  }
  else return Registrar(COD_INSTRUCCION_INEXISTENTE, simbolos->valores[*i]);
  return COD_OK;
}

/*
 * Analizador sintáctico. Itera sobre un conjunto de símbolos para generar un árbol del programa
 * @param simbolos Objeto Array con los símbolos generados por alexico
 * @param estructuras Objeto blqModulo con el AST del programa
 */
ling_codigomsg asintactico(Array *simbolos, blqModulo *estructuras){
  int i;
  // Programa debe ser la primera instrucción
  if (strcmp(simbolos->valores[0], "Programa")) return Registrar(COD_PROGRAMA_PRIMER);
  else {
    estructuras->nombre = iniciarCadena();
    if (simbolos->tipos[1] != SIMBOLO_EXPRESION) return Registrar(COD_INSTRUCCION_INCOMPLETA, simbolos->valores[1]);
    anadirSCadena(&estructuras->nombre, simbolos->valores[1]);
  }

  for (i=2; i<simbolos->tamano; i++){
    cjtInstruccion *inst;
    ling_codigomsg err = constInstruccion(simbolos, &i, &inst);
    if (err) return err;
    anadirArray(&estructuras->instrucciones, inst, 0);
  }
  return COD_OK;
}

/*
 * Función encargada de ejecutar cada instrucción
 * @param instruccion Instrucción a ejecutar
 * @param tipoPadre El tipo de instrucción que contiene a la instrucción a ejecutar
 * @param variables Objeto Dict que contiene las variables declaradas
 * @param regresar Variable auxiliar para el comando Regresar
 */
ling_codigomsg ejecInstruccion(cjtInstruccion *instruccion, ling_codigotipo tipoPadre, Dict *variables){
  int i;
  int pos;
  cjtExpresion *val;
  ling_codigomsg cod;
  switch(instruccion->tipo){
  case BLOQUE_SI:;
    Registrar(COD_INICIO_CONDICIONAL, prog.pasoAPaso);
    cod = aseExpresion(&instruccion->expresion, &val, variables);
    if (cod) return cod;

    if ((val->tipo == VALOR_NUMERO && val->Num != 0) || (val->tipo == VALOR_CADENA && strlen(val->Cad)) || (val->tipo == VALOR_LOGICO && val->Bool == true)){
      for (i=0; i<instruccion->instrucciones.tamano;i++){
        cod = ejecInstruccion(instruccion->instrucciones.valores[i], BLOQUE_SI, variables);
        if (cod) return cod;
      }
    } else {
      for (i=0; i<instruccion->instruccionesn.tamano;i++){
        cod = ejecInstruccion(instruccion->instruccionesn.valores[i], BLOQUE_SI, variables);
        if (cod) return cod;
      }
    }
    Registrar(COD_FIN_CONDICIONAL, prog.pasoAPaso);
    return COD_OK;
  case BLOQUE_MIENTRAS:;
    Registrar(COD_INICIO_MIENTRAS, prog.pasoAPaso);
    cod = aseExpresion(&instruccion->expresion, &val, variables);
    if (cod) return cod;
    while ((val->tipo == VALOR_NUMERO && val->Num != 0) || (val->tipo == VALOR_CADENA && strlen(val->Cad)) || (val->tipo == VALOR_LOGICO && val->Bool == true)){
      for (i=0; i<instruccion->instrucciones.tamano;i++){
        cod = ejecInstruccion(instruccion->instrucciones.valores[i], BLOQUE_MIENTRAS, variables);
        if (cod == COD_CONTINUAR || cod == COD_SALIDA) break;
        else if (cod == COD_FIN_SALIDA) ;
        else if (cod) return cod;
      }
      aseExpresion(&instruccion->expresion, &val, variables);
      if (cod == COD_SALIDA) return COD_FIN_SALIDA;
      if (cod == COD_CONTINUAR) continue;
    }
    Registrar(COD_FIN_MIENTRAS, prog.pasoAPaso);
    return COD_OK;
  case SENTENCIA_DEFINIR:;
    if (enArray(instruccion->variable, &variables->identificadores)>-1) return Registrar(COD_REDEFINICION, instruccion->variable);
    else {
      val = malloc(sizeof(cjtExpresion));
      if (instruccion->tipodato == VALOR_NUMERO){
        Registrar(COD_SENTENCIA_NUMERO, prog.pasoAPaso, instruccion->variable);
        val->tipo = VALOR_NUMERO;
        val->Num = 0;
      }
      else if (instruccion->tipodato == VALOR_CADENA){
        Registrar(COD_SENTENCIA_CADENA, prog.pasoAPaso, instruccion->variable);
        val->tipo = VALOR_CADENA;
        val->Cad = iniciarCadena();
      }
      else if (instruccion->tipodato == VALOR_LOGICO){
        Registrar(COD_SENTENCIA_LOGICO, prog.pasoAPaso, instruccion->variable);
        val->tipo = VALOR_LOGICO;
        val->Bool = false;
      }
      anadirArray(&variables->valores, val, 0);
      anadirArray(&variables->identificadores, instruccion->variable, 0);
    }
    break;
  case SENTENCIA_ASIGNARA:;
    pos = enArray(instruccion->variable, &variables->identificadores);
    if (pos==-1) return Registrar(COD_VARIABLE_INDEFINIDA, instruccion->variable);
    cod = aseExpresion(&instruccion->expresion, &val, variables);
    if (cod) return cod;
    cjtExpresion *var = (cjtExpresion*) variables->valores.valores[pos];
    if (var->tipo != val->tipo) return Registrar(COD_TIPO_INCOMPATIBLE, cadenaTipo(var->tipo), cadenaTipo(val->tipo));
    if (val->tipo == VALOR_NUMERO) Registrar(COD_SENTENCIA_ASIGNARA_NUMERO, prog.pasoAPaso, instruccion->variable, val->Num);
    else if (val->tipo == VALOR_CADENA) Registrar(COD_SENTENCIA_ASIGNARA_CADENA, prog.pasoAPaso, instruccion->variable, val->Cad);
    else if (val->tipo == VALOR_LOGICO) Registrar(COD_SENTENCIA_ASIGNARA_LOGICO, prog.pasoAPaso, instruccion->variable, val->Bool);
    variables->valores.valores[pos] = val;
    return COD_OK;
  case SENTENCIA_LEER:;
    pos = enArray(instruccion->variable, &variables->identificadores);
    if (pos==-1) return Registrar(COD_VARIABLE_INDEFINIDA, instruccion->variable);
    val = (cjtExpresion*) variables->valores.valores[pos];
    if (val->tipo != VALOR_CADENA) return Registrar(COD_LEER_VARIABLE_CADENA);
    free(val->Cad);
    val->Cad = calloc(1, 1);
    char c = (char) getchar();
    while (c != '\n'){anadirCadena(&val->Cad, c); c = (char) getchar();}
    return COD_OK;
  case SENTENCIA_ESCRIBIR:;
    Registrar(COD_SENTENCIA_ESCRIBIR, prog.pasoAPaso);
    cod = aseExpresion(&instruccion->expresion, &val, variables);
    if (cod) return cod;
    if (val->tipo != VALOR_CADENA) return Registrar(COD_CADENA_IMPLICITA);
    puts(val->Cad);
    return COD_OK;
  case SENTENCIA_CONTINUAR:;
    if (tipoPadre == BLOQUE_MODULO) return Registrar(COD_SALTO_IMPROCEDENTE);
    return Registrar(COD_CONTINUAR, prog.pasoAPaso);
  case SENTENCIA_REGRESAR:;
    if (tipoPadre == BLOQUE_MODULO) return Registrar(COD_SALTO_IMPROCEDENTE);
    return Registrar(COD_SALIDA, prog.pasoAPaso);
  case SENTENCIA_SALIR:;
    cod = aseExpresion(&instruccion->expresion, &val, variables);
    if (!cod){
      prog.estado = (int) val->Num;
      return Registrar(COD_ABORTO, prog.pasoAPaso);
    }
    else return cod;
  }
  return COD_OK;
}

/*
 * Intérprete
 * @param estructuras Objeto blqModulo que contiene todas las instrucciones a ejecutar
 */
ling_codigomsg asemantico(blqModulo *estructuras){
  Dict variables = iniciarDict();
  int i;
  Registrar(COD_INICIO_PROGRAMA, prog.pasoAPaso, estructuras->nombre);
  for (i=0; i<estructuras->instrucciones.tamano;i++){
    ling_codigomsg cod = ejecInstruccion(estructuras->instrucciones.valores[i], BLOQUE_MODULO, &variables);
    if (cod == COD_FIN_SALIDA) ;
    else if (cod) return cod;
  }
  Registrar(COD_FIN_PROGRAMA, prog.pasoAPaso, estructuras->nombre);
  return COD_OK;
}

int main(int argc, char *argv[]){
  FILE *archivo = NULL;
  prog.pasoAPaso = false;
  prog.estado = 0;

  if (argc == 1) return Registrar(COD_NOARCHIVO);
  else if (!strcmp(argv[1], "-a")){
    puts("Uso: linguini [-i] archivo");
    puts("Opciones:");
    puts("-a\tMuestra esta ayuda");
    puts("-i\tEjecuta el script paso a paso el programa y va explicando qué va haciendo");
    puts("-v\tMuestra información sobre el programa");
    return 0;
  }
  else if (!strcmp(argv[1], "-v")){
    puts("Linguini 1.0");
    puts("Copyright (c) 2020 Giovanni Alfredo Garciliano Díaz");
    puts("Este programa está bajo la Licencia Pública General de GNU versión 3.0, o posterior");
    puts("Hecho para un proyecto de la asignatura de Fundamentos de Programación, en la Facultad de Ingeniería de la UNAM");
    return 0;
  }
  else if (argc == 3 && !strcmp(argv[1], "-i")){
    prog.pasoAPaso = true;
    archivo = fopen(argv[2], "rb");
  }
  else if (argc == 2 && !strcmp(argv[1], "-i")) return Registrar(COD_PARAMETROS);
  else if (argc == 2) archivo = fopen(argv[1], "rb");
  else return Registrar(COD_PARAMETROS);
  if (!archivo){
    return Registrar(COD_NOARCHIVO);
  } else {
    Registrar(COD_COMPILACION, prog.pasoAPaso);
    Array simbolos = iniciarArray();
    blqModulo estructuras;
    estructuras.instrucciones = iniciarArray();

    ling_codigomsg elexico = alexico(archivo, &simbolos);
    fclose(archivo);
    if (elexico) return elexico;

    ling_codigomsg esintactico = asintactico(&simbolos, &estructuras);
    if (esintactico) return esintactico;

    ling_codigomsg esemantico = asemantico(&estructuras);
    if (esemantico) return esemantico;
  }
  return 0;
}
