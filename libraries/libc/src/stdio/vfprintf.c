/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#include <errno.h>
#include <malloc.h>
#include <stdio.h>

int vfprintf(FILE* stream, const char* format, va_list ap) {
  if (stream == NULL) return -EINVAL;
  if (stream->write == NULL) return -ENOSYS;
  va_list tmpap;
  va_copy(tmpap, ap);
  char tmp;
  int r = vsnprintf(&tmp, 1, format, tmpap);
  if (r < 0) return r;
  char* str = malloc(r);
  if (str == NULL) return r;
  vsnprintf(str, r, format, ap);
  r = stream->write(stream, str, r);
  if (r > 0) stream->offset += r;
  free(str);
  va_end(tmpap);
  va_end(ap);
  return r;
}
