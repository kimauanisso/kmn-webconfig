#include "kmn_webconfig/variables.h"

VariableRegistry varRegistry = {.variables = {}, .variableCount = 0};

void webconfvariable_register(WebConfigVar variable) {
  if (varRegistry.variableCount >= VARIABLE_COUNT) {
    return;
  }

  varRegistry.variables[varRegistry.variableCount++] = variable;
}