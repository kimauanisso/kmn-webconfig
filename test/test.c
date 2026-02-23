#include "kmn_webconfig/config.h"
#include "kmn_webconfig/handlers/rootHandler.h"
#include "kmn_webconfig/handlers/routes.h"
#include "kmn_webconfig/http.h"
#include "kmn_webconfig/http_response.h"
#include "kmn_webconfig/router.h"
#include "kmn_webconfig/variables.h"

#include <stdio.h>
#include <string.h>

#define VARIABLE_COUNT 16

char req[MAX_BUFFER_SIZE];
HttpBuffer res = {.buffer = {}, .used = 0, .size = MAX_BUFFER_SIZE};

int a = 10;
int b = 1;
int c = 0;
float d = 0.5f;
WebConfigVar var1 = {.id = 0,
                     .name = "TestVariable",
                     .kind = INT,
                     .selector = SLIDER,
                     .value = &a};
WebConfigVar var2 = {.id = 1,
                     .name = "TestBool",
                     .kind = BOOL,
                     .selector = CHECKBOX,
                     .value = &b};
WebConfigVar var3 = {.id = 2,
                     .name = "TestBool2",
                     .kind = BOOL,
                     .selector = CHECKBOX,
                     .value = &c};
WebConfigVar var4 = {
    .id = 3, .name = "TestFloat", .kind = FLOAT, .selector = TEXT, .value = &d};

char request[] = "GET /definitions HTTP/1.1\n"
                 "HeaderHere: asdasd";

int main() {
  init_routes();
  webconfvariable_register(var1);
  webconfvariable_register(var2);
  webconfvariable_register(var3);
  webconfvariable_register(var4);
  handleRoute(request, "asdasd", &res);
  printf("%s\n", res.buffer);
}