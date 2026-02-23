#pragma once

#include "config.h"
#include "http.h"
#include "http_response.h"

typedef void (*WebConfigRouteHandler)(HttpBuffer *req, HttpBuffer *res);

typedef struct RouterConfig {
  Route route;
  WebConfigRouteHandler handler;
} RouterConfig;

void registerRoute(RouterConfig route);
void handleRoute(HttpBuffer *request, HttpBuffer *result);