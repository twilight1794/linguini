#include "args.h"

int Args_Procesar(int argc, char** argv, Dicc **d){
  if (argc < 2){
    *d = NULL;
    return (int) ARGS_NOPARAM;
  }
  /* Rellenar parámetros predeterminados */
  *d = Dicc_Crear();
  bool *vald = Tiposdin_C_Bool(false);
  Dicc_Anadir(*d, "d", vald);
  int *valf = Tiposdin_C_Int(495);
  Dicc_Anadir(*d, "f", valf);
  bool *valh = Tiposdin_C_Bool(false);
  Dicc_Anadir(*d, "h", valh);
  char *vali = NULL;
  Dicc_Anadir(*d, "i", vali);
  char *valm = NULL);
  Dicc_Anadir(*d, "m", valm);
  char *valo = NULL;
  Dicc_Anadir(*d, "o", valo);
  bool *valv = Tiposdin_C_Bool(false);
  Dicc_Anadir(*d, "v", valv);

  /* Personalizar */
  size_t i;
  int s;
  for (i=1; i<argc; i++){
    if (!strncmp(argv[i], "-d", 2)){
      Tiposdin_R_Bool(&vald, true);
      Dicc_Anadir(*d, "d", vald);

    } else if (!strncmp(argv[i], "-f", 2)){
      i++;
      if (i == argc) return (int) ARGS_PARAM_INCOMPLETO;
      int numf = (int) strtol(argv[i],  NULL, 0);
      if (numf>1023) s = (int) ARGS_VAL_F_INVALIDO;
      else {
	Tiposdin_R_Int(&valf, numf);
	Dicc_Anadir(*d, "f", valf);
      }

    } else if (!strncmp(argv[i], "-h", 2)){
      Tiposdin_R_Bool(&valh, true);
      Dicc_Anadir(*d, "h", valh);

    } else if (!strncmp(argv[i], "-i", 2)){
      i++;
      if (i == argc) return (int) ARGS_PARAM_INCOMPLETO;
      Tiposdin_R_String(&vali, argv[i]);
      Dicc_Anadir(*d, "i", vali);

    } else if (!strncmp(argv[i], "-m", 2)){
      i++;
      if (i == argc) return (int) ARGS_PARAM_INCOMPLETO;
      LDE* l = LDE_Crear();
      size_t t = strlen(argv[i])+1;
      char* cad = (char*) malloc(sizeof(char)*t);
      strncopy(cad, argv[i], t);
      char* simb = strtok(cad, ",");
      while (simb != NULL){
	LDE_AnadirPosterior(l, simb);
        simb = strtok(NULL, "-");
      }
      Dicc_Anadir(*d, "m", l);

    } else if (!strncmp(argv[i], "-o", 2)){
      i++;
      if (i == argc) return (int) ARGS_PARAM_INCOMPLETO;
      Tiposdin_R_String(&valo, argv[i]);
      Dicc_Anadir(*d, "o", valo);

    } else if (!strncmp(argv[i], "-v", 2)){
      Tiposdin_R_Bool(&valv, true);
      Dicc_Anadir(*d, "v", valv);

    } else s = (int) ARGS_PARAM_DESCONOCIDO;
  }
  return s;
}
