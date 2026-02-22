#pragma once

enum WebConfVarKind {
  INT,
  FLOAT,
  BOOL
};

enum WebConfSelector {
  TEXT,
  SLIDER,
  CHECKBOX,
  NONE
};

typedef struct WebConfigVar
{
  const int id;
  const char* name;
  const WebConfVarKind kind;
  const WebConfSelector selector;
  void *value;
} WebConfigVar;