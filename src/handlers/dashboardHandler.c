#include "kmn_webconfig/handlers/dashboardHandler.h"
#include "index_html.h"

void dashHandler(char *req, char *params, ResponseBuffer *res) {
  build_res_start(res, 200, "OK");
  build_res_header(res, "Content-Type", "text/html; charset=utf-8");
  res_buffer_append(res, "Content-Length: %d\n", index_html_len);
  build_res_header(res, "Connection", "close");
  res_buffer_append(res, "\n");
  res_buffer_append(res, index_html);
}