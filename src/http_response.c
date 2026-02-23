#include "kmn_webconfig/http_response.h"

#include "kmn_webconfig/config.h"
#include <stdio.h>

void build_res_start(int code, char *message, char *dest) {
  snprintf(dest, sizeof(char) * MAX_RES_SIZE, "HTTP/1.1 %03i %s\n", code,
           message);
}