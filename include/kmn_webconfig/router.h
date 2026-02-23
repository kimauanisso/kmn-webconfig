#pragma once

#include "config.h"
#include "http.h"
#include "http_response.h"

typedef void (*WebConfigRouteHandler)(HttpBuffer *req, HttpBuffer *res);

typedef struct RouterConfig {
  Route route;
  WebConfigRouteHandler handler;
} RouterConfig;

/// @brief Adds a route into router registry
/// @param route Route to be added
void registerRoute(RouterConfig route);

/// @brief Parses and handles request for a given route
/// @param request Request buffer
/// @param result Response buffer
void handleRoute(HttpBuffer *request, HttpBuffer *result);