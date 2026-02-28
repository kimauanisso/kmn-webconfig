#include "kmn_webconfig/handlers/variableHandler.h"

#include "kmn_webconfig/variables.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *getBodyStart(HttpBuffer *request) {
  for (size_t i = 0; i + 3 < request->buffer; i++) {
    if (request->buffer[i] == '\r' && request->buffer[i + 1] == '\n' &&
        request->buffer[i + 2] == '\r' && request->buffer[i + 3] == '\n') {
      return request->buffer + i + 4;
    }
  }
  return NULL;
}

void variableNotFoundError(HttpBuffer *res, int id) {
  build_res_start(res, 404, "Not Found");
  build_res_header(res, "Connection", "close");
  res_buffer_append(res, "\r\n");
  res_buffer_append(res, "{\"message\": \"variable '%d' not found\"}", id);
}

void badRequestError(HttpBuffer *res, const char *message) {
  build_res_start(res, 400, "Bad Request");
  build_res_header(res, "Connection", "close");
  res_buffer_append(res, "\r\n");
  res_buffer_append(res, "{\"message\": \"%s\"}", message);
}

void okResponse(HttpBuffer *res) {
  build_res_start(res, 200, "OK");
  build_res_header(res, "Connection", "close");
  res_buffer_append(res, "\r\n");
}

void variableHandler(HttpBuffer *req, HttpBuffer *res) {
  char *bodyStart = getBodyStart(req);
  if (bodyStart == NULL) {
    badRequestError(res, "missing body");
    return;
  }
  printf("Parsing Body: %s\n", bodyStart);

  char *line = strtok(bodyStart, "\n");

  if (strncmp(line, "id=", 3) != 0) {
    badRequestError(res, "id not found");
    return;
  }

  int id = atoi(line + 3);
  WebConfigVar *var = webconfvariable_get_by_id(id);

  if (var == NULL) {
    variableNotFoundError(res, id);
    return;
  }

  line = strtok(NULL, "\n");
  if (strncmp(line, "value=", 6) != 0) {
    badRequestError(res, "value not found");
    return;
  }

  char *valueStr = line + 6;
  char *endptr;

  switch (var->kind) {
  case INT: {
    int errno = 0;
    long v = strtol(valueStr, &endptr, 10);

    if (errno != 0 || *endptr != '\0') {
      badRequestError(res, "failed parsing int");
      return;
    }

    int *valPtr = var->value;
    *valPtr = (int)v;
    break;
  }

  case FLOAT: {
    int errno = 0;
    float v = strtof(valueStr, &endptr);

    if (errno != 0 || *endptr != '\0') {
      badRequestError(res, "failed parsing float");
      return;
    }

    float *valPtr = var->value;
    *valPtr = v;
    break;
  }

  case BOOL: {
    if (strcmp(valueStr, "true") == 0) {
      int *valPtr = var->value;
      *valPtr = 1;
    } else if (strcmp(valueStr, "false") == 0) {
      int *valPtr = var->value;
      *valPtr = 0;
    } else {
      badRequestError(res, "failed parsing bool");
      return;
    }
    break;
  }
  }
  okResponse(res);
}