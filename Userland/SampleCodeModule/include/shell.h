#ifndef SHELL_H
#define SHELL_H
#include <commands.h>

void initShell();

typedef struct {
  command module;
  char** args;
  unsigned int argCount;
} caller;

#endif