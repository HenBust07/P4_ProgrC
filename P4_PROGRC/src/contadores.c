#include "contadores.h"

int Curr(int CurrSequence, uint8_t ultimaPosicion,int secuenciaSeleccionada){
  
   // para en el sentido original, evitar desbordamiento
        if ((CurrSequence >= ultimaPosicion)&&(secuenciaSeleccionada == 2)){
            CurrSequence = 0;
        }
        // para el sentido inverso, evitar desbordamiento
        if ((CurrSequence < 0)&&(secuenciaSeleccionada == 1)){
            CurrSequence = ultimaPosicion;
        }

      
  return CurrSequence;
}

