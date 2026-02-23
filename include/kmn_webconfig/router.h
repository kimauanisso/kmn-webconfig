#pragma once

#include "config.h"
#include "http.h"
#include "http_response.h"

typedef void (*WebConfigRouteHandler)(char *req, char *param, HttpBuffer *res);

typedef struct RouterConfig {
  Route route;
  WebConfigRouteHandler handler;
} RouterConfig;

void registerRoute(RouterConfig route);
void handleRoute(char *request, char *params, HttpBuffer *result);