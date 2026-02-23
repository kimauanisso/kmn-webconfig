#pragma once

#include "kmn_webconfig/router.h"

#include "kmn_webconfig/handlers/dashboardHandler.h"
#include "kmn_webconfig/handlers/definitionsHandler.h"
#include "kmn_webconfig/handlers/rootHandler.h"

static const Route _rootGetRoute = {.method = GET, .path = "/"};
const RouterConfig rootGet = {.route = _rootGetRoute, .handler = rootHandler};

static const Route _dashGetRoute = {.method = GET, .path = "/webconfig"};
const RouterConfig dashGet = {.route = _dashGetRoute, .handler = dashHandler};

static const Route _defGetRoute = {.method = GET, .path = "/definitions"};
const RouterConfig defGet = {.route = _defGetRoute,
                             .handler = definitionsHandler};

void init_routes() {
  registerRoute(rootGet);
  registerRoute(dashGet);
  registerRoute(defGet);
}