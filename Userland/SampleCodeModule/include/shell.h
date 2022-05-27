#ifndef SHELL_H
#define SHELL_H
#include <commands.h>

typedef enum {
  RUNNING, PAUSED, ENDED
} STATUS;

void runShell();
void initShell();


typedef struct caller {
  int (*runner)(unsigned int count, void** args);
  void** args;
  unsigned int argCount;
  unsigned int screenId;
} caller;

#endif