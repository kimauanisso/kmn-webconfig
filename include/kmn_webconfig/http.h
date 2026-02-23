#pragma once

// HTTP Methods
#define HTTP_GET "GET"
#define HTTP_POST "POST"
#define HTTP_PUT "PUT"
#define HTTP_PATCH "PATCH"
#define HTTP_DELETE "DELETE"

typedef enum HttpMethod { GET, POST, PUT, PATCH, DELETE } HttpMethod;

typedef struct Route {
  HttpMethod method;
  const char *path;
} Route;

Route parseRoute(char *firstline);