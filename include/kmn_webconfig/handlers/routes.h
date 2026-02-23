#pragma once

#include "kmn_webconfig/router.h"

#include "kmn_webconfig/handlers/rootHandler.h"

static const Route _rootGetRoute = {.method = GET, .path = "/"};
const RouterConfig rootGet = {.route = _rootGetRoute, .handler = rootHandler};

void init_routes() { registerRoute(rootGet); }