#include "kmn_webconfig/http.h"

#include <stdio.h>
#include <string.h>

/// @brief Parses HTTP method into HttpMethod enum
/// @param method Request's method string
/// @return Result enum
static HttpMethod parseMethod(char *method) {
  if (!strcmp(method, HTTP_GET))
    return GET;

  if (!strcmp(method, HTTP_POST))
    return POST;

  if (!strcmp(method, HTTP_PUT))
    return PUT;

  if (!strcmp(method, HTTP_PATCH))
    return PATCH;

  if (!strcmp(method, HTTP_DELETE))
    return DELETE;

  return -1;
}

/// @brief Parses a request's first line into a route definition
/// @param firstline Request's first line string
/// @return Route definition
Route parseRoute(char *firstline) {
  char *method = strtok(firstline, " ");
  char *path = strtok(NULL, " ");
  Route route = {.method = parseMethod(method), path};
  return route;
}