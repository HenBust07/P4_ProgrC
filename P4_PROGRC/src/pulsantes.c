#include "pulsantes.h"

int leerTecla(int16_t tecla, const char* nombreTecla, int Intervalo, int cont){
int val = !digitalRead(tecla);
int val2 = 0;
int bandera = 0;
int valout = val;
if (val != val2){
    val2 = val;
    cont=0;
    bandera = 1;
}

//Segundo estado DOWN->UP o UP->DOWN
if ((bandera == 1) && (cont == Intervalo)){
  //se cuenta 50ms para confirmar que se quiere cambiar de estado
    //Primer estado DOWN: Comienza en 0
  if ((val == 0)){ //bandera = 1 significa que se realizó un cambio
    valout = 0;
  }
  //Tercer estado UP
  if (val == 1){//bandera = 1 significa que se realizó un cambio
    valout = 1;
  }
  bandera = 0;
  cont=0;
}
if (cont >500){ //evitar trabajar con valores grandes con el timer1
  cont = 0;
}
    return valout;
}

int prioridad(int16_t tecla1,int16_t tecla2,int16_t tecla3,int16_t tecla4){
  int SW;
  if (tecla2 == 1){
      SW = 1;
  } else if (tecla1 == 1){
      SW = 0;
  } else if (tecla3 == 1){
      SW = 2;
  } else if (tecla4 == 1){
      SW = 3;
  } else{
      SW = 8;
  }
  return SW;
}