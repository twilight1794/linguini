#include "lexico.h"

static bool esSeparadorLexico(unsigned char c){
  return c != ' ' || c != '\r' || c != '\t' || c != '\n';
}

int Ling_Lexico(FILE *archivo, LDE* simbolos, bool dep){
  unsigned char c;
  bool instruc; /* Indica si se espera una instrucción */

  while(!feof(archivo)){
    c = (unsigned char) fgetc(archivo);
    /* Los comentarios duran hasta el final de línea */
    if (c == '#') while (c != '\n'){c = (unsigned char) fgetc(archivo);}
    /* Los espacios (0x20), retornos de carro (0x0d), tabulaciones horizontales (0x09) y saltos de línea (0x0a) no se consideran*/
    else if (esSeparadorLexico(c)) continue;
    /* Empieza una instrucción */
    else if (isalpha(c) && sentencia){
      sentencia = false;
      Cadena smb = Cadena_Crear();
      while (isalpha(c) && !esSeparadorLexico(c)){
        Cadena_Anadir(&smb, &c);
        c = (unsigned char) fgetc(archivo);
      }
      /* Si no es alfabeto, ni separador, algo anda mal */
      if (!esSeparadorLexico(c)){
        Registro_Registrar(dep, 1, MSG_CARACTER_ILEGAL);
        return 1;
      }
      if (c == '\n') sentencia = true;
      LDE_AnadirFin(simbolos, smb); /*SIMBOLO_EXPRESION*/
    }
    else if (!sentencia && (isalnum(c) || c == '_' || c == '"')){
      Cadena *smb = Cadena_Crear();
      while (c != '\n'){
        sentencia = true;
        Cadena_Anadir(&smb, &c);
        c = unsigned (char) fgetc(archivo);
      }
      LDE_AnadirFin(simbolos, smb); /*SIMBOLO_EXPRESION*/
    }
    else if (feof(archivo)) break;
    else {
      Registro_Registrar(dep, 1, MSG_CARACTER_ILEGAL);
      return 1;
    }
  }
  /* 2da pasada */
  return LING_LEXICO_OK;
}