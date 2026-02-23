#include "kmn_webconfig/http_response.h"

#include "kmn_webconfig/config.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

BufferOperationStatus res_buffer_append(HttpBuffer *buf, const char *fmt, ...) {
  va_list args;

  size_t available = buf->size - buf->used;

  va_start(args, fmt);
  int needed = vsnprintf(NULL, 0, fmt, args);
  va_end(args);

  if (needed < 0) {
    return FORMAT_ERROR;
  }

  if ((size_t)needed + 1 > available) {
    return OUT_OF_MEMORY;
  }

  va_start(args, fmt);
  vsnprintf(buf->buffer + buf->used, available, fmt, args);
  va_end(args);

  buf->used += needed;
  buf->buffer[buf->used] = '\0';
}

void build_res_start(HttpBuffer *buf, int code, char *message) {
  res_buffer_append(buf, "HTTP/1.1 %i %s\n", code, message);
}

void build_res_header(HttpBuffer *buf, char *header, char *value) {
  res_buffer_append(buf, "%s: %s\n", header, value);
}