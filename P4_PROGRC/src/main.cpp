#include <Arduino.h>
#include <stdint.h>

#define LED_GREEN 11
#define LED_YELLOW 10
#define LED_RED 9
#define LED_BLUE 8

#define SW1 2
#define SW2 3
#define SW3 4
#define SW4 5

/*extern "C"{
    #include "leds.h"
    #include "pulsantes.h"
}*/

int interval; //microsegundos
int prevCount;
int currCount;

int ton; 
int tcurr;

bool flag;

int seq;

typedef int gpioMap_t;
const gpioMap_t secuencia[] = {LED_GREEN, LED_YELLOW, LED_RED, LED_BLUE};
const uint8_t ultimoLed = sizeof(secuencia)/sizeof(gpioMap_t);

int currSeq;

int sw1val;
int sw2val;
int sw3val;
int sw4val;

int swSelec; //Sw seleccionado para evitar el doble accionar de dos o más pulsantes

bool encenderLed(int16_t led){
	switch(led){
  case LED_YELLOW:
        digitalWrite(led, HIGH);
        break;
	case LED_RED:
        digitalWrite(led, HIGH);
        break;
	case LED_GREEN:
        digitalWrite(led, HIGH);
        break;
	case LED_BLUE:
		digitalWrite(led, HIGH);
        break;
  default:
        return false;
    }
    return true;
}

bool apagarLed(){
	digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
}

bool leerTecla(int16_t tecla, const char* nombreTecla){
    bool val = !digitalRead(tecla);
    bool val2 = 0;
    if (val != val2){
    Serial.print(nombreTecla);
    Serial.print(": ");
    Serial.println(val);
    val2 = val;
    }
    return val;
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



void setup(){
    Serial.begin(115200);
    pinMode(SW1, INPUT_PULLUP);
    pinMode(SW2, INPUT_PULLUP);
    pinMode(SW3, INPUT_PULLUP);
    pinMode(SW4, INPUT_PULLUP);

	  pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    interval = 10000;
    prevCount = 0;
    currCount = 0;

    ton = 25;
    tcurr = ton;

    flag = 0;
    //sw2val2 = 0;
}

void loop(){
  
    
    currCount = micros();
    if ((currCount -prevCount) >= interval){
        if(flag == true ){
            flag = false;
            apagarLed();
            int led = secuencia[currSeq];
            encenderLed(led);
            currSeq++;
            
        }else{
           // apagarLed() ; //apagatodos los leds
        }
        if (tcurr == ton){
            tcurr = 0;
            flag = true;
        }else{
            tcurr++;
        }
        if (currSeq >= ultimoLed){
            currSeq = 0;
        }
        
        
         sw1val = leerTecla(SW1, "SW1");
         sw2val = leerTecla(SW2, "SW2");
         sw3val = leerTecla(SW3, "SW3");
         sw4val = leerTecla(SW4, "SW4");
        
        //prioridad
        swSelec = prioridad(sw1val,sw2val,sw3val,sw4val);
      

        switch(swSelec){
          case 0:
            Serial.println("Pulsante seleccionado SW1");
            break;
          case 1:
            Serial.println("Pulsante seleccionado SW2");
            break;
          case 2:
            Serial.println("Pulsante seleccionado SW3");
            break;
          case 3:
            Serial.println("Pulsante seleccionado SW4");
            
            break;
          default:
              break;
        }


        prevCount = currCount;
         
    }
   
}
