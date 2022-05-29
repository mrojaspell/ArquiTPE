#ifndef COMMANDS_H
#define COMMANDS_H
#define COMMANDS_LENGTH 12
#define DUMP_SIZE 32
#define LAST_MEM 0x80000000

#include <stdbool.h>

/* 
  Los programas validos son estos:
  Que reciban la cantidad de variables, los argumentos en un array de strings
  Tienen que devolver si termino su ejecucion o no. 1 si termino, 0 sino.
*/ 

typedef struct {
  char *name;
  int (*runner)(int count, void** args);
  char *description;
  void (*initFunction)(int screenId);
} command;

command* getCommands();

/*
 Lista de comandos y su estructura
 command {
   char* name;        NOMBRE DE LA FUNCION
   void (*runner)();  FUNCION A CORRER
 } 
*/

extern int _opcodeExp();

int dateAndTime();
int help();
int infoReg();
int holaMundo();
int printmem(int argc, char* argv[]);
int divZero();
int invalidOpcode();
int fibonacci();
int prime(int argc, char* argv[], int screenId);
void emptyInit(int screenId);
int wait(int argc, char* argv[]);

#endif