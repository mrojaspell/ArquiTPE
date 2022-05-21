#ifndef COMMANDS_H
#define COMMANDS_H
#define COMMANDS_LENGTH 7
#define DUMP_SIZE 32


/* 
  Los programas validos son estos:
  Que reciban la cantidad de variables, los argumentos en un array de strings
  Tienen que devolver si termino su ejecucion o no. 1 si termino, 0 sino.
*/ 

typedef struct {
  char *name;
  int (*runner)(int count, char** args);
  char *description;
} command;

command* getCommands(int* size);

/*
 Lista de comandos y su estructura
 command {
   char* name;        NOMBRE DE LA FUNCION
   void (*runner)();  FUNCION A CORRER
 } 
*/

extern int _opcodeExp();

void dateAndTime();
void help();
void infoReg();
void holaMundo();
void printmem(char* address);
void divZero();
void invalidOpcode();


#endif