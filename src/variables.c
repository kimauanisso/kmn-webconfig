#include "kmn_webconfig/variables.h"

#include <stdlib.h>

VariableRegistry varRegistry = {.variables = {}, .variableCount = 0};

void webconfvariable_register(WebConfigVar variable) {
  if (varRegistry.variableCount >= VARIABLE_COUNT) {
    return;
  }

  varRegistry.variables[varRegistry.variableCount++] = variable;
}

WebConfigVar* webconfvariable_get_by_id(int id) {
  for (int i = 0; i < varRegistry.variableCount; i++)
  {
    int currentId = varRegistry.variables[i].id;
    if(currentId == id){
      return &varRegistry.variables[i];
    }
  }

  return NULL;
}