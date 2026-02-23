#pragma once

#include "kmn_webconfig/config.h"

// HTTP Methods
#define HTTP_GET "GET"
#define HTTP_POST "POST"
#define HTTP_PUT "PUT"
#define HTTP_PATCH "PATCH"
#define HTTP_DELETE "DELETE"

typedef enum HttpMethod { GET, POST, PUT, PATCH, DELETE } HttpMethod;

typedef struct HttpBuffer {
  char buffer[MAX_BUFFER_SIZE];
  unsigned int size;
  unsigned int used;
} HttpBuffer;

typedef struct Route {
  HttpMethod method;
  const char *path;
} Route;

Route parseRoute(char *firstline);