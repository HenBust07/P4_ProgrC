#include <Arduino.h>
#include <stdint.h>
 //Modularización
extern "C"{
    #include "leds.h"
    #include "pulsantes.h"
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
const gpioMap_t secuencia[] = {LED_GREEN, LED_YELLOW, LED_RED}; //mediante esta estructura definimos la sec1
const uint8_t primerLed = secuencia[0]; //obtenemos la longitud de la estructura 1
const uint8_t ultimoLed = sizeof(secuencia)/sizeof(gpioMap_t); //obtenemos la longitud de la estructura 1
typedef int gpioMap_t2; //definimos un tipo de dato
const gpioMap_t2 secuencia2[] = {LED_BLUE};//mediante esta estructura definimos la sec2
const uint8_t primerLed2 = secuencia2[0]; //obtenemos la longitud de la estructura 1
const uint8_t ultimoLed2 = sizeof(secuencia2)/sizeof(gpioMap_t2); //obtenemos la longitud de la estructura 2
int currSeq; //Variable para saber en que posición de la secuencia nos encontramos
//punteros
int * led; 
int *ultimo;
int *primero;


//para leer los valores de los switch
int sw1val;
int sw2val;
int sw3val;
int sw4val;
int swSelec; //Sw seleccionado para evitar el doble accionar de dos o más pulsantes
int secuenciaSelec; //Se guarda la elección entre el sw1 y sw2 para saber que secuencia mostrar



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
    //sw2val2 = 0;
    secuenciaSelec = 2;
    //inicializamos punteros
    led = secuencia;
    ultimo = secuencia[ultimoLed];
    primero = primerLed;
}

void loop(){ 
    
    currCount = micros();
    if ((currCount -prevCount) >= interval){
        if(flag == true ){ //indicamos el encendido y apagado de cada led para la percepción de las secuencias
            flag = false;
            apagarLed(); //apagamos leds que no corresponden al estado 
            //int led = secuencia[currSeq]; //obtenemos un dato a la vez de la secuencia1
            int led2 = secuencia2[currSeq]; //lo mismo en la secuencia 2
            //encendemos los leds que corresponden al estado actual
            encenderLed(*led); 
            encenderLed(led2);
            //avanzamos en la estructura de las secuencias
            if (secuenciaSelec == 2){ //sentido original
              //currSeq++;
              led++;
            } else if (secuenciaSelec == 1){ //sentido inverso
              //currSeq--;
              led--;
            }
            
        }else{
           // apagarLed() ; //apagatodos los leds
        }
        //controlamos los tiempos en que permanecen encendidos los leds
        if (tcurr >= ton){
            tcurr = 0;
            flag = true;
        }else{
            tcurr++;
        }
        // para en el sentido original, evitar desbordamiento
        /*if ((currSeq >= ultimoLed)&&(secuenciaSelec == 2)){
            currSeq = 0;
        }*/
        if ((led == &secuencia[ultimoLed])&&(secuenciaSelec == 2)){
            led = secuencia;
        }

        // para el sentido inverso, evitar desbordamiento
        if ((led < &secuencia[0])&&(secuenciaSelec == 1)){
            led = &secuencia[ultimoLed];
        }
        //leemos los valores de los pulsantes
         sw1val = leerTecla(SW1, "SW1");
         sw2val = leerTecla(SW2, "SW2");
         sw3val = leerTecla(SW3, "SW3");
         sw4val = leerTecla(SW4, "SW4");
        
        //prioridad
        swSelec = prioridad(sw1val,sw2val,sw3val,sw4val);

        switch(swSelec){
          case 0: // Sentido inverso
            Serial.println("Pulsante seleccionado SW1");
            secuenciaSelec = 1;

            break;
          case 1: //Original
            Serial.println("Pulsante seleccionado SW2");
            secuenciaSelec = 2;
            break;
          case 2: //200ms
            Serial.println("Pulsante seleccionado SW3");
            ton = 20;
            break;
          case 3: //750ms
            Serial.println("Pulsante seleccionado SW4");
            ton = 75;
            
            break;
          default:
              break;
        }
        prevCount = currCount; //igualamos el tiempo previo con el actual para seguir con el siguiente intervalo
         
    }
   
}
