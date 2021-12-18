/**
 * @file linguini.c
 * @brief Compilador
 */

#include <stdio.h>

#include "base/diccionario.h"

#include "args.h"
#include "expresiones.h"

int main(int argc, char* argv[]){
  FILE *entrada = NULL;
  FILE *salida = NULL;

  Dicc *args;
  void *arg;

  /* Ayuda */
  int est = Args_Procesar(argc, argv, &args);
  Dicc_Obtener(args, "h", &arg);
  if (((bool) *arg) == true){
    puts("Uso: linguini [-d] [-f 0-1023] [-h] [-i <archivo>] [-m <rutas>] [-o <archivo>] [-v]");
    puts("Opciones:");
    puts("-d\tMuestra mensajes de depuraci�n");
    puts("-f\tN�mero para configurar las banderas");
    puts("-h\tMuestra esta ayuda");
    puts("-i\tArchivo de entrada. Predeterminadamente, la entrada est�ndar");
    puts("-m\tRuta para buscar m�dulos. Predeterminadamente, el directorio actual");
    puts("-o\tArchivo de salida. Predeterminadamente, la salida est�ndar");
    puts("-v\tMuestra informaci�n sobre el programa");
    return 0;
  }

  
  return 0;
}
