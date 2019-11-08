/**
  * StardustOS libc - (C) 2019 Tristan Ross
  */
#include <math.h>
#include <stdlib.h>
#include <string.h>

int ftoa(float value, char* ptr, int dig) {
  if (ptr == NULL) return 0;
  int ipart = (int)value;
  float fpart = value - (float)ipart;
  int i = itoa(ptr, 10, ipart);
  if (dig != 0) {
    ptr[i++] = '.';
    fpart *= pow(10, dig);
    i += itoa(ptr + i, 10, (int)fpart);
  }
  return i;
}