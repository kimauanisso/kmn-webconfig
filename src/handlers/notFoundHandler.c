#include "kmn_webconfig/handlers/notFoundHandler.h"

#include "kmn_webconfig/http_response.h"

void notFoundHandler(HttpBuffer *req, HttpBuffer *res) {
  build_res_start(res, 404, "Not Found");
  build_res_header(res, "Connection", "close");
  res_buffer_append(res, "\r\n");
}