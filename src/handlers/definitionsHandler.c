#include "kmn_webconfig/handlers/definitionsHandler.h"

#include "kmn_webconfig/variables.h"

static char *selector_strings[] = {[TEXT] = "text",
                                   [SLIDER] = "slider",
                                   [CHECKBOX] = "checkbox",
                                   [NONE] = "none"};

static char *kind_strings[] = {
    [INT] = "int", [FLOAT] = "float", [BOOL] = "bool", [ACTION] = "action"};

static char *true_string = "true";
static char *false_string = "false";

static void build_variable(HttpBuffer *res, WebConfigVar variable) {
  char *kind = kind_strings[variable.kind];
  char *selector = selector_strings[variable.selector];

  res_buffer_append(res, "{");
  res_buffer_append(res, "\"id\":%i,", variable.id);
  res_buffer_append(res, "\"type\":\"%s\",", kind);
  res_buffer_append(res, "\"selector\":\"%s\",", selector);

  switch (variable.kind) {
  case INT: {
    int *value = (int *)variable.value;
    res_buffer_append(res, "\"value\":%i", *value);
  } break;
  case FLOAT: {
    float *value = (float *)variable.value;
    res_buffer_append(res, "\"value\":%f", *value);
  } break;
  case BOOL:
    int *rawValue = (int *)variable.value;
    char *str = *rawValue ? true_string : false_string;
    res_buffer_append(res, "\"value\":%s", str);

  default:
    break;
  }
  res_buffer_append(res, "}");
}

void definitionsHandler(HttpBuffer *req, HttpBuffer *res) {
  build_res_start(res, 200, "OK");
  build_res_header(res, "Content-Type", "application/json");

  // Response body
  res_buffer_append(res, "\r\n");
  res_buffer_append(res, "{\"definitions\":[");

  // Build webconfig variables
  for (int i = 0; i < varRegistry.variableCount; i++) {
    build_variable(res, varRegistry.variables[i]);
    if (i != varRegistry.variableCount - 1) {
      res_buffer_append(res, ",");
    }
  }

  res_buffer_append(res, "]}");
}