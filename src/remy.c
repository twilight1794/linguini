// Linguini 2.0.0
// Máquina virtual Remy
// GPLv3

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

//#include <unistd.h>

//#include "expresiones.h"

int main(int argc, char **argv){
  if (argc == 1) return ling_Registrar(COD_NOARCHIVO, NULL);
  else if (!strcmp(argv[1], "-a")) return ling_Imprimir(TXT_AYUDA);
  else if (!strcmp(argv[1], "-v")) return ling_Imprimir(TXT_VERSION);
  else {
    FILE *archivo = fopen(argv[2], "rb");
    if (!archivo) return Registrar(COD_NOARCHIVO);
  }
}