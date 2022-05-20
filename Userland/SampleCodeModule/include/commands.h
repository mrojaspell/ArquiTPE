#ifndef COMMANDS_H
#define COMMANDS_H
#define COMMANDS_LENGTH 4


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
void dateAndTime();
void help();
void infoReg();
void holaMundo();

#endif