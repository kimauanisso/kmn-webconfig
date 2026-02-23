#include "kmn_webconfig/handlers/notFoundHandler.h"

#include "kmn_webconfig/http_response.h"

void notFoundHandler(char *req, char *params, ResponseBuffer *res) {
  build_res_start(res, 404, "Not Found");
}