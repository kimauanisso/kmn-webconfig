#pragma once

#include "kmn_webconfig/config.h"

typedef enum BufferOperationStatus {
  OK,
  OUT_OF_MEMORY,
  FORMAT_ERROR
} BufferOperationStatus;

typedef struct ResponseBuffer {
  char buffer[MAX_RES_SIZE];
  unsigned int size;
  unsigned int used;
} ResponseBuffer;

BufferOperationStatus res_buffer_append(ResponseBuffer *buf, const char *fmt,
                                        ...);

void build_res_start(ResponseBuffer *buf, int code, char *message);