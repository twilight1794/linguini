/**
 * @file mensajes_es.h
 * @brief Localización para Linguini en español
 */

#ifndef L11N_ES_INC
#define L11N_ES_INC

#define _(A, B) const char* S_##A = B;

_(INSTRUCCION_INEXISTENTE, "Instrucción %s inexistente")
_(INSTRUCCION_INCOMPLETA, "Instrucción %s incompleta")
_(PALABRA_RESERVADA, "%s es una palabra reservada y no puede usarse como un identificador")
_(MULTIPLE_PROGRAMA, "%s solo puede ir una vez al principio del programa")
_(SALIR_NOBUCLE, "%s solo se puede usar para salir de un bucle")
_(REGRESAR_NOBLOQUE, "%s solo se puede usar para finalizar un bloque")
_(NOMODULO, "%s no es un módulo")
_(NOPROGRAMA, "%s no es un programa")
_(PARAMETROS_INVALIDOS, "La función %s necesita %u parámetros, pero has pasado %u")
_(IDENTIFICADOR_INVALIDO, "Identificador %s inválido")
_(SIN_MEMORIA"Memoria insuficiente")
_(ERROR_ES, "El archivo no pudo abrirse")
_(ERROR_IGNOTO, "Hubo un error desconocido")
};

#endif
