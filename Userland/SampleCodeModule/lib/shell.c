#include <ustdlib.h>
#include <shell.h>
#include <ctype.h>
#include <usersyscalls.h>
#include <RTCID.h>
#include <commands.h>

#define BUFFER_LENGTH 256


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


char* getCommandLine() {
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
  int length;
  command* commands = getCommands(&length);
  for (int i = 0; i < length; i += 1) {
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
    char *commandLine = getCommandLine();
    _putc(STDOUT, '\n');
    runCommand(commandLine);
    _putc(STDOUT, '\n');
  }
}