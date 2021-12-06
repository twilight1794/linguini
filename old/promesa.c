#include "promesa.h"

void Promesa_PonerCodigo(Promesa *p, int c){
  p->codigo = c;
}
void Promesa_PonerValorP(Promesa *p, void* v){
  p->val.valp = v;
}
void Promesa_PonerValorI(Promesa *p, int v){
  p->val.vali = v;
}
void Promesa_PonerValorC(Promesa *p, char v){
  p->val.valc = v;
}
void Promesa_PonerValorF(Promesa *p, float v){
  p->val.valf = v;
}

int Promesa_Codigo(Promesa* p){
  return p->codigo;
}
void* Promesa_ValorP(Promesa* p){
  return p->val.valp;
}
int Promesa_ValorI(Promesa* p){
  return p->val.vali;
}
char Promesa_ValorC(Promesa* p){
  return p->val.valc;
}
float Promesa_ValorF(Promesa* p){
  return p->val.valf;
}