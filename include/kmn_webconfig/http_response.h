#pragma once

#include "kmn_webconfig/config.h"
#include "kmn_webconfig/http.h"

typedef enum BufferOperationStatus {
  OK,
  OUT_OF_MEMORY,
  FORMAT_ERROR
} BufferOperationStatus;

/// @brief Safely appends formatted string to HttpBuffer content
BufferOperationStatus res_buffer_append(HttpBuffer *buf, const char *fmt, ...);

/// @brief Generates HTTP response first line
/// @param buf Destination buffer
/// @param code Response status code
/// @param message Response message
void build_res_start(HttpBuffer *buf, int code, char *message);

/// @brief Generates HTTP response header entry
/// @param buf Destination buffer
/// @param header Header name
/// @param value Header value
void build_res_header(HttpBuffer *buf, char *header, char *value);