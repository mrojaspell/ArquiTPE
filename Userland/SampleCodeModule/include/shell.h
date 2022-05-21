#ifndef SHELL_H
#define SHELL_H
#include <commands.h>

typedef enum {
  RUNNING, PAUSED, ENDED
} STATUS;

void initShell();

typedef struct {
  command program;
  char** args;
  unsigned int argCount;
} caller;

#endif