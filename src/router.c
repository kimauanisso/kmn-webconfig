#include "kmn_webconfig/router.h"

#include <stdio.h>
#include <string.h>

#include "kmn_webconfig/handlers/notFoundHandler.h"

typedef struct Router {
  int routeCount;
  RouterConfig routes[MAX_ROUTES];
} Router;

Router routeHandler = {.routeCount = 0, .routes = {}};

static WebConfigRouteHandler getHandler(Route route) {
  for (int i = 0; i < routeHandler.routeCount; i++) {
    RouterConfig routeConfig = routeHandler.routes[i];

    int isSameMethod = route.method == routeConfig.route.method;
    int isSamePath = strcmp(route.path, routeConfig.route.path) == 0;

    if (isSameMethod && isSamePath) {
      return routeConfig.handler;
    }
  }

  return NULL;
}

void registerRoute(RouterConfig route) {
  if (routeHandler.routeCount == MAX_ROUTES) {
    return;
  }

  routeHandler.routes[routeHandler.routeCount] = route;
  routeHandler.routeCount++;
}

void handleRoute(char *request, char *params, HttpBuffer *result) {
  char *line = strtok(request, "\n");
  Route route = parseRoute(line);

  if (route.method == -1) {
    notFoundHandler(request, params, result);
    return;
  }

  WebConfigRouteHandler handler = getHandler(route);

  if (handler == NULL) {
    notFoundHandler(request, params, result);
    return;
  }

  handler(request, params, result);
}