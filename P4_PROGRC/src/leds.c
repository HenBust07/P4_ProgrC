#include "leds.h"
int encenderLed(int16_t led){
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
        return 0;
    }
    return 1;
}

bool apagarLed(){
	digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    return true;
}