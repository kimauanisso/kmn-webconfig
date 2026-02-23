#include "kmn_webconfig/handlers/rootHandler.h"

void rootHandler(HttpBuffer *req, HttpBuffer *res) {
  build_res_start(res, 302, "Redirect");
  build_res_header(res, "Location", "192.168.4.1/main");
}