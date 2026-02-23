#include "kmn_webconfig/handlers/notFoundHandler.h"

#include "kmn_webconfig/http_response.h"

void notFoundHandler(char *req, char *params, char *res) {
  build_res_start(404, "Not Found", res);
}