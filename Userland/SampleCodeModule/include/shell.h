#ifndef SHELL_H
#define SHELL_H

typedef struct {
  char *name;
  void (*runner)();
} command;

void initShell();

#endif