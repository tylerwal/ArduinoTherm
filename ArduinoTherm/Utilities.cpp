#include "Arduino.h"
#include "Utilities.h"

bool CompareStrings(const char* one, const char* two)
{
	return strcmp(one, two) == 0;
}

char* FloatToString(float input)
{
	char buffer[6];
	
	itoa(input * 10, buffer, 10);
	int inputLength = strlen(buffer);
	buffer[inputLength + 1] = buffer[inputLength];
	buffer[inputLength] = buffer[inputLength - 1];
	buffer[inputLength - 1] = '.';

	return buffer;
}

void StartPin(int pinNumber)
{
	digitalWrite(pinNumber, HIGH);
}

void StopPin(int pinNumber)
{
	digitalWrite(pinNumber, LOW);
}

// code @ https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
int freeRam()
{
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
