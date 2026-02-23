#pragma once

#include "kmn_webconfig/config.h"

typedef enum WebConfVarKind { INT, FLOAT, BOOL, ACTION } WebConfVarKind;

typedef enum WebConfSelector { TEXT, SLIDER, CHECKBOX, NONE } WebConfSelector;

typedef struct WebConfigVar {
  int id;
  char *name;
  WebConfVarKind kind;
  WebConfSelector selector;
  void *value;
} WebConfigVar;

typedef struct VariableRegistry {
  WebConfigVar variables[VARIABLE_COUNT];
  int variableCount;
} VariableRegistry;

extern VariableRegistry varRegistry;

/// @brief Registers a WebConfigVar into registry
/// @param variable Variable to be registered
/// @warning The variable will not be registered if registry's variable count
/// exceeds VARIABLE_COUNT
void webconfvariable_register(WebConfigVar variable);