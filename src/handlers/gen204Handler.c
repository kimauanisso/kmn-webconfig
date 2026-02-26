#include "kmn_webconfig/handlers/gen204Handler.h"

void gen204Handler(HttpBuffer *req, HttpBuffer *res) {
  build_res_start(res, 302, "Redirect");
  build_res_header(res, "Location", "http://192.168.4.1/webconfig");
  res_buffer_append(res, "\r\n");
}