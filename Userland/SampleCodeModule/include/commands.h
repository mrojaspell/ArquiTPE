#ifndef COMMANDS_H
#define COMMANDS_H
#define COMMANDS_LENGTH 7
#define DUMP_SIZE 32

typedef struct {
  char *name;
  void (*runner)();
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