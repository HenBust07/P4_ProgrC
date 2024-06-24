#include <Arduino.h>
#include <stdint.h>
#include <TimerOne.h>

 //Modularización
extern "C"{
    #include "leds.h"
    #include "pulsantes.h"
    #include "contadores.h"
}
//Variables para los retardos no bloqueantes
int interval; //microsegundos
int prevCount;
int currCount;
//Variables para el tiempo de encendido de los LEDS
int ton; 
int tcurr;
//Indicador booleano para condicionales
bool flag;
//para la o las secuencias
typedef int gpioMap_t; //definimos un tipo de dato
// Definimos una estructura que contiene el arreglo secuencia
typedef struct {
    const gpioMap_t* leds;
    const uint8_t size;
} LedSequence;
// Creamos e inicializamos la estructura de la secuencia 1
gpioMap_t secuenciaArray1[] = {LED_GREEN};
LedSequence secuencia1 = {secuenciaArray1, sizeof(secuenciaArray1) / sizeof(gpioMap_t)};
// Utilizamos punteros para obtener el primer y último LED
const gpioMap_t* primerLed1 = secuencia1.leds; // Puntero al primer LED
const uint8_t ultimoLed1 = secuencia1.size;    // Tamaño del arreglo
// Creamos e inicializamos la estructura de la secuencia 2
gpioMap_t secuenciaArray2[] = {LED_YELLOW, LED_RED, LED_BLUE};
LedSequence secuencia2 = {secuenciaArray2, sizeof(secuenciaArray2) / sizeof(gpioMap_t)};
// Utilizamos punteros para obtener el primer y último LED
const gpioMap_t* primerLed2 = secuencia2.leds; // Puntero al primer LED
const uint8_t ultimoLed2 = secuencia2.size;    // Tamaño del arreglo
//Contadores
int currSeq; //Variable para saber en que posición de la secuencia nos encontramos
int currSeq2; //Variable para saber en que posición de la secuencia nos encontramos
//para leer los valores de los switch
int sw1val;
int sw2val;
int sw3val;
int sw4val;
int swSelec; //Sw seleccionado para evitar el doble accionar de dos o más pulsantes
int secuenciaSelec; //Se guarda la elección entre el sw1 y sw2 para saber que secuencia mostrar
//Para el antirebote
int SWin; //valor del switch ingresa
int SWout; //valor del switch controlado antirebote
int bandera; //Se cumplió el estado de transición
int cont; //Contador para la interrupción
int Intervalo; //tiempo de tolerancia para confirmar el cambio de estados
int SWanterior;
//interrupción
void timerDelay() {
  //Serial.print(cont);
  cont++;
}
void setup(){
    Serial.begin(115200);
    //declaración de entradas y salidas
    pinMode(SW1, INPUT_PULLUP);
    pinMode(SW2, INPUT_PULLUP);
    pinMode(SW3, INPUT_PULLUP);
    pinMode(SW4, INPUT_PULLUP);
	  pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    //inicialización de variables
    interval = 10000;
    prevCount = 0;
    currCount = 0;
    ton = 50; //tiempo por defecto
    tcurr = ton;
    flag = 0;
    currSeq = 0;
    currSeq2 = 0;
    secuenciaSelec = 2;
    //Inicialización para antirebote
    SWin = 0; // 
    SWout = 0; //
    Intervalo = 50; // En este caso son 50 milisegundos
    cont = 0;
    bandera = 0;
    SWanterior=0;
  
    Timer1.initialize(1000); // Configura el timer para 1000 microsegundos es igual a 1 milisegundo
                           // Para que sea visual se puede probar con 5 segundos, reemplazar 1000000
    Timer1.attachInterrupt(timerDelay); // Asocia la interrupción a la función retencion
}
void loop(){ 
   //   printf("currSeq1= %d\n");
 currCount = micros(); 
    if ((currCount - prevCount) >= interval) {
        if (flag) {
            flag = false;
            apagarLed();  // Apaga todos los LEDs antes de encender los correspondientes
            encenderLed(secuenciaArray1[currSeq]);  // Enciende el LED correspondiente
            encenderLed(secuenciaArray2[currSeq2]);
            
            // Avanza o retrocede en la secuencia dependiendo de la dirección
            if (secuenciaSelec == 2) {  // Sentido original
                currSeq++;
                currSeq2++;
            } else if (secuenciaSelec == 1) {  // Sentido inverso
                currSeq--;
                currSeq2--;
            }
      //Funcion para evitar el desbordamiento
       currSeq = Curr(currSeq, ultimoLed1, secuenciaSelec);
       currSeq2 = Curr(currSeq2, ultimoLed2, secuenciaSelec);
      }
        // Controla los tiempos en que permanecen encendidos los LEDs
      if (tcurr >= ton) {
            tcurr = 0;
            flag = true;
      } else {
            tcurr++;
      } 
      // Lee los valores de los pulsantes (simulado)
      int sw1val = leerTecla(SW1, "SW1", Intervalo, cont);
      int sw2val = leerTecla(SW2, "SW2", Intervalo, cont);
      int sw3val = leerTecla(SW3, "SW3", Intervalo, cont);
      int sw4val = leerTecla(SW4, "SW4", Intervalo, cont);





      // Prioridad de los pulsantes (simulado)
      int swSelec = prioridad(sw1val, sw2val, sw3val, sw4val);   
      // Cambia la secuencia según el pulsante seleccionado
      switch (swSelec) {
          case 0:  // Sentido inverso
              Serial.println("Pulsante seleccionado SW1");
              secuenciaSelec = 1;
              break;
          case 1:  // Sentido original
              Serial.println("Pulsante seleccionado SW2");
              secuenciaSelec = 2;
              break;
          case 2:  // 200ms
              Serial.println("Pulsante seleccionado SW3");
              ton = 20;
              break;
           case 3:  // 750ms
              Serial.println("Pulsante seleccionado SW4");
              ton = 75;
               break;
           default:
              break;
      }
      prevCount = currCount; //igualamos el tiempo previo con el actual para seguir con el siguiente intervalo
   }
}

