#pragma once

#include "config.h"
#include "http.h"

typedef void (*WebConfigRouteHandler)(char *req, char *param, char *res);

typedef struct RouterConfig {
  Route route;
  WebConfigRouteHandler handler;
} RouterConfig;

void registerRoute(RouterConfig route);
void handleRoute(char *request, char *params, char *result);