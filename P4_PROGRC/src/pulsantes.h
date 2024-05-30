#ifndef __PULSANTES_H 
#define __PULSANTES_H
#include <Arduino.h>
#include <stdint.h>
#include "hal.h"
int leerTecla(int16_t tecla, const char* nombreTecla);
int prioridad(int16_t tecla1,int16_t tecla2,int16_t tecla3,int16_t tecla4);
#endif