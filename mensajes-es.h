#include <stdarg.h>

typedef enum {
              COD_OK = 0,
              // Registro de depuracion
              COD_COMPILACION = 101, COD_INICIO_PROGRAMA, COD_FIN_PROGRAMA, COD_INICIO_FUNCION, COD_FIN_FUNCION, COD_INICIO_CONDICIONAL, COD_FIN_CONDICIONAL, COD_INICIO_MIENTRAS, COD_FIN_MIENTRAS, COD_CONTINUAR, COD_SALIDA, COD_FIN_SALIDA /* <-no es un bloque, pero marca el fin de la prorpagación del comando */, COD_ABORTO, COD_SENTENCIA_NUMERO, COD_SENTENCIA_CADENA, COD_SENTENCIA_LOGICO, COD_SENTENCIA_ASIGNARA_NUMERO, COD_SENTENCIA_ASIGNARA_CADENA, COD_SENTENCIA_ASIGNARA_LOGICO, COD_SENTENCIA_LEER, COD_SENTENCIA_ESCRIBIR,
              // Errores de sintaxis
              COD_INSTRUCCION_INEXISTENTE = 201, COD_INSTRUCCION_INCOMPLETA, COD_PALABRA_RESERVADA, COD_PROGRAMA_MULTIPLE, COD_PROGRAMA_PRIMER, COD_SALTO_IMPROCEDENTE, COD_FIN_IMPROCEDENTE, COD_EXPRESION_IMPROCEDENTE, COD_LEER_VARIABLE_CADENA, COD_CARACTER_ILEGAL, COD_EXPRESION_MALFORMADA, COD_PARENTESIS_EXCEDENTE,
              // Errores de expresiones
              COD_VARIABLE_INDEFINIDA = 301, COD_REDEFINICION, COD_TIPO_INCOMPATIBLE, COD_OPERADOR_INCOMPATIBLE, COD_CADENA_IMPLICITA, COD_PARAMETROS_INSUFICIENTES, COD_PARAMETROS_EXCESIVOS, COD_EXPRESION_INDEFINIDA, COD_IDENTIFICADOR_INVALIDO,
              // Errores del sistema
              COD_MEMORIA_INSUFICIENTE = 401,
              // Situaciones especiales
              COD_DESCONOCIDO = 501,
              // Errores de compilación
              COD_NOARCHIVO = 601, COD_PARAMETROS
} ling_codigomsg;

ling_codigomsg Registrar(ling_codigomsg c, ...){
  va_list params;
  bool d = false;
  va_start(params, c);
  if (c > 200)fprintf(stderr, "Error %d: ", c);
  else {
    d = va_arg(params, bool);
    if (d) fprintf(stderr, "Registro: ");
  }
  if (c == COD_COMPILACION && d) fprintf(stderr, "Compilando programa\n");
  else if (c == COD_INICIO_PROGRAMA && d) fprintf(stderr, "Inicio del programa %s\n", va_arg(params, char*));
  else if (c == COD_FIN_PROGRAMA && d) fprintf(stderr, "Fin del programa %s\n", va_arg(params, char*));
  else if (c == COD_INICIO_FUNCION && d) fprintf(stderr, "Inicio de la función %s\n", va_arg(params, char*));
  else if (c == COD_FIN_FUNCION && d) fprintf(stderr, "Fin de la función %s\n", va_arg(params, char*));
  else if (c == COD_INICIO_CONDICIONAL && d) fprintf(stderr, "Inicio del condicional Si\n");
  else if (c == COD_FIN_CONDICIONAL && d) fprintf(stderr, "Fin del condicional Si\n");
  else if (c == COD_INICIO_MIENTRAS && d) fprintf(stderr, "Inicio del bucle Mientras\n");
  else if (c == COD_FIN_MIENTRAS && d) fprintf(stderr, "Fin del bucle Mientras\n");
  else if (c == COD_CONTINUAR && d) fprintf(stderr, "Salto a la siguiente iteración\n");
  else if (c == COD_SALIDA && d) fprintf(stderr, "Salida del bucle\n");
  else if (c == COD_ABORTO && d) fprintf(stderr, "Salida prematura del programa\n");
  else if (c == COD_SENTENCIA_NUMERO && d) fprintf(stderr, "Declaración de la variable %s de tipo Numero\n", va_arg(params, char*));
  else if (c == COD_SENTENCIA_CADENA && d) fprintf(stderr, "Declaración de la variable %s de tipo Cadena\n", va_arg(params, char*));
  else if (c == COD_SENTENCIA_LOGICO && d) fprintf(stderr, "Declaración de la variable %s de tipo Logico\n", va_arg(params, char*));
  else if (c == COD_SENTENCIA_ASIGNARA_NUMERO && d) fprintf(stderr, "Asignación del número %f a la variable %s\n", va_arg(params, double), va_arg(params, char*));
  else if (c == COD_SENTENCIA_ASIGNARA_CADENA && d) fprintf(stderr, "Asignación de la cadena %s a la variable %s\n", va_arg(params, char*), va_arg(params, char*));
  else if (c == COD_SENTENCIA_ASIGNARA_LOGICO && d) fprintf(stderr, "Asignación del valor lógico %s a la variable %s\n", (d?"Verdadero":"Falso"), va_arg(params, char*));
  else if (c == COD_SENTENCIA_LEER && d) fprintf(stderr, "Leer en la variable %s\n", va_arg(params, char*));
  else if (c == COD_SENTENCIA_ESCRIBIR && d) fprintf(stderr, "Escribir en la pantalla\n");
  else if (c == COD_INSTRUCCION_INEXISTENTE)
    fprintf(stderr, "La instrucción %s no existe\n", va_arg(params, char*));
  else if (c == COD_INSTRUCCION_INCOMPLETA)
    fprintf(stderr, "La instrucción %s está incompleta\n", va_arg(params, char*));
  else if (c == COD_PALABRA_RESERVADA)
    fprintf(stderr, "La palabra %s está reservada y no se puede utilizar como identificador\n", va_arg(params, char*));
  else if (c == COD_PROGRAMA_MULTIPLE)
    fprintf(stderr, "Solo puede haber una sentencia Programa en el archivo\n");
  else if (c == COD_PROGRAMA_PRIMER)
    fprintf(stderr, "La sentencia Programa debe ser la primera sentencia en el archivo\n");
  else if (c == COD_SALTO_IMPROCEDENTE)
    fprintf(stderr, "La sentencia Continuar o Regresar no puede usarse fuera de un bucle\n");
  else if (c == COD_FIN_IMPROCEDENTE)
    fprintf(stderr, "La sentencia Fin solo puede usarse para terminar un bloque Si o uno Mientras\n");
  else if (c == COD_EXPRESION_IMPROCEDENTE)
    fprintf(stderr, "Una expresión no puede ser usada en las sentencias Numero, Cadena, Logico, Fin o Regresar\n");
  else if (c == COD_LEER_VARIABLE_CADENA)
    fprintf(stderr, "La lectura de la entrada estándar solo puede guardarse en una variable de tipo Cadena\n");
  else if (c == COD_CARACTER_ILEGAL)
    fprintf(stderr, "El caracter %x no puede existir como parte de una sentencia\n", va_arg(params, int));
  else if (c == COD_EXPRESION_MALFORMADA)
    fprintf(stderr, "La expresión es inválida\n");
  else if (c == COD_PARENTESIS_EXCEDENTE)
    fprintf(stderr, "Hay paréntesis de más en la expresión\n");
  else if (c == COD_VARIABLE_INDEFINIDA)
    fprintf(stderr, "La variable %s no está definida\n", va_arg(params, char*));
  else if (c == COD_REDEFINICION)
    fprintf(stderr, "La variable %s ya está definida\n", va_arg(params, char*));
  else if (c == COD_TIPO_INCOMPATIBLE)
    fprintf(stderr, "No se puede asignar una expresión de tipo %s a una variable de tipo %s\n", va_arg(params, char*), va_arg(params, char*));
  else if (c == COD_OPERADOR_INCOMPATIBLE)
    fprintf(stderr, "El operador o funcion %s solo opera con valores de tipo %s\n", va_arg(params, char*), va_arg(params, char*));
  else if (c == COD_CADENA_IMPLICITA)
    fprintf(stderr, "Un valor de tipo Numero o Logico no puede ser usado como cadena\n");
  else if (c == COD_PARAMETROS_INSUFICIENTES)
    fprintf(stderr, "Faltan parámetros en la función %s\n", va_arg(params, char*));
  else if (c == COD_PARAMETROS_EXCESIVOS)
    fprintf(stderr, "Hay demasiados parámetros en la función %s\n", va_arg(params, char*));
  else if (c == COD_EXPRESION_INDEFINIDA)
    fprintf(stderr, "La expresión no está definida en los números reales\n");
  else if (c == COD_IDENTIFICADOR_INVALIDO)
    fprintf(stderr, "Identificador '%s' inválido\n", va_arg(params, char*));
  else if (c == COD_MEMORIA_INSUFICIENTE)
    fprintf(stderr, "Memoria insuficiente\n");
  else if (c == COD_DESCONOCIDO)
    fprintf(stderr, "Hubo un error desconocido\n");
  else if (c == COD_NOARCHIVO)
    fprintf(stderr, "No se puede abrir el archivo\n");
  else if (c == COD_PARAMETROS)
    fprintf(stderr, "Debes especificar un archivo, y un parámetro opcional. Consulte la ayuda ejecutando ./linguini -a\n");
  va_end(params);
  return c;
}
