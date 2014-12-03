#ifndef UTILITIES_H
#define UTILITIES_H

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

bool CompareStrings(const char* one, const char* two);

char* FloatToString(float input);

int freeRam();

#endif /* UTILITIES_H */