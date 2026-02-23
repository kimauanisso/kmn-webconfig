#include "kmn_webconfig/http.h"

#include <stdio.h>
#include <string.h>

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

Route parseRoute(char *firstline) {
  char *method = strtok(firstline, " ");
  char *path = strtok(NULL, " ");
  Route route = {.method = parseMethod(method), path};
  return route;
}