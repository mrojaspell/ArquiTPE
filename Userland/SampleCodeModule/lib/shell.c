#include <ustdlib.h>
#include <shell.h>
#include <ctype.h>
#include <usersyscalls.h>

#define BUFFER_LENGTH 256
#define COMMANDS_LENGTH 3


/*
 Lista de comandos y su estructura
 command {
   char* name;        NOMBRE DE LA FUNCION
   void (*runner)();  FUNCION A CORRER
 } 
*/
void help();
void infoReg();
void holaMundo();

//ponerlos en orden alfabetico plssssssss
static command commands[COMMANDS_LENGTH] = {{ "hello", &holaMundo }, { "help", &help }, { "inforeg", &infoReg }};


void help(){
  _fprint(STDOUT, "Lista de posibles comandos: \n");
  for (uint8_t i = 0; i < COMMANDS_LENGTH; i++){
    // _fprintf(STDOUT, "\t%s\n", commands[i].name); CORREGIR fprintf
    _fprint(STDOUT, "\t");
    _fprint(STDOUT, commands[i].name);
    _fprint(STDOUT, "\n");
  }
}

void infoReg(){
  static uint64_t registers[16]; //despues arreglar tamanio
  sys_inforeg(registers, (sizeof(registers)/sizeof(registers[0])));
}

void holaMundo() {
  _fprintf(STDOUT, "hola mundoss %s", "hola");
  return;
}

// recibe una linea de commando y te devuelve si es ese commando o no
int isCommand(const char* commandLine, const char* command) {
  int cl_i = 0;
  int c_i = 0;
  while (commandLine[cl_i] == ' ') {
    cl_i += 1;
  }
  while (command[c_i] != '\0' && commandLine[cl_i] != '\0') {
    if (toLower(commandLine[cl_i]) != toLower(command[c_i])) return 0;
    cl_i += 1;
    c_i += 1;
  }

  return command[c_i] == '\0' && (commandLine[cl_i] == ' ' || commandLine[cl_i] == '\n' || commandLine[cl_i] == '\0');
}


char* getCommand() {
  static char buffer[BUFFER_LENGTH];

  int c;
  int i = 0;
  while ((c = getChar()) != '\n') {
    if (c != '\b' && i < BUFFER_LENGTH - 1) {
      buffer[i++] = c;
      _putc(STDOUT, c);
    } else if (i > 0) {
        i--;
        _putc(STDOUT, '\b');
    }
  }
  buffer[i] = '\0';
  return buffer;
}

void runCommand(const char *str) {
  for (int i = 0; i < COMMANDS_LENGTH; i += 1) {
    if (isCommand(str, commands[i].name)) {
      return commands[i].runner();
    }
  }
  _fprint(STDOUT, "Este comando no es valido");
  return;
}

void initShell() {
  clear_screen(1);
  while (1) {
    _putc(STDOUT, '>');
    char *commandLine = getCommand();
    _putc(STDOUT, '\n');
    runCommand(commandLine);
    _putc(STDOUT, '\n');
  }
}