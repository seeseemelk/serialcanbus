#include "board.h"

#include <Arduino.h>

void ledOn()
{
	digitalWrite(LED_AUX, 1);
}

void ledOff()
{
	digitalWrite(LED_AUX, 0);
}
