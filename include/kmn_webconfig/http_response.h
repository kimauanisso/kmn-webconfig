#pragma once

#include "kmn_webconfig/config.h"
#include "kmn_webconfig/http.h"

typedef enum BufferOperationStatus {
  OK,
  OUT_OF_MEMORY,
  FORMAT_ERROR
} BufferOperationStatus;

BufferOperationStatus res_buffer_append(HttpBuffer *buf, const char *fmt, ...);

void build_res_start(HttpBuffer *buf, int code, char *message);
void build_res_header(HttpBuffer *buf, char *header, char *value);