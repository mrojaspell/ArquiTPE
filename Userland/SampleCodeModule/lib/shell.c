#include <ustdlib.h>
#include <shell.h>
#include <ctype.h>
#include <usersyscalls.h>
#include <RTCID.h>
#include <commands.h>

#define BUFFER_LENGTH 256
#define MAX_ARGS 20

/* TEMP
  'c' para volver a modo una pantalla
  '1' para suspender o activar primer programa
  '2' para suspender o activar segundo programa
*/

int eventLoop(caller* callers, int programCount) {
  STATUS firstProgram = RUNNING;
  STATUS secondProgram = (programCount != 2) ? (ENDED) : (RUNNING);

  // TODO: esperar a poder cancelar
  int isPipe = programCount == 2;
  if (programCount == 2) {
    sys_toggleMode(1);
    sys_clear(STDOUT);
  }

  while (!(firstProgram == ENDED && secondProgram == ENDED) || isPipe) {
    // FALTA HACER METODO DE CANCELAR EJECUCION
    // int c = 0;
    // if (c == 'c') {
    //   break;
    // } else if (c == '1') {
    //   runFirstProgram = !runFirstProgram;
    // } else if (c == '2') {
    //   runSecondProgram = !runSecondProgram;
    // }

    if (firstProgram == RUNNING) {
      if (programCount == 2) {
        sys_switchScreen(1);
      }
      int ended = callers[0].program.runner(callers[0].argCount, callers[0].args);
      if (ended) firstProgram = ENDED;
    }
    if (secondProgram == RUNNING) {
      if (programCount == 2) {
        sys_switchScreen(2);
      }
      int ended = callers[1].program.runner(callers[1].argCount, callers[1].args);
      if (ended) secondProgram = ENDED;
    }
  }

  if (programCount == 2) {
    sys_switchScreen(0);
    sys_toggleMode(0);
    sys_clear(STDOUT);
  }
  return 0;
}

// Devuelve strings con delimitador de ' '.
// strings es el array de char*

int getCommandLine(char** strings) {
  static char buffer[BUFFER_LENGTH];
  buffer[BUFFER_LENGTH - 1] = '\0'; // Por las dudas

  char* startString = buffer; // Guarda el puntero al string que esta obteniendo
  int c;
  int i = 0;
  int res_index = 0;
  while ((c = getChar()) != '\n' && i < BUFFER_LENGTH - 1 && res_index < MAX_ARGS) {
    switch(c) {
      case '\b':
        if (i != 0) {
          i--;
          _putc(STDOUT, c);
        }
        break;
      case ' ':
        buffer[i++] = '\0';
        strings[res_index++] = startString;
        startString = buffer + i;
        _putc(STDOUT, c);
        break;
      default:
        buffer[i++] = c;
        _putc(STDOUT, c);
    }
  }
  if (i == 0) return 0;
  buffer[i] = '\0'; //Resuelve el ultimo string presente
  strings[res_index++] = startString;
  return res_index;
}

int runCommandLine(int argCount, char** args) {
  if (argCount == 0) return 0;
  int pipeIndex = 0;
  int stop = 0;
  for (int i = 0; i < argCount && !stop; i += 1) {
    if (_strcasecmp(args[i], "|")) {
      pipeIndex = i;
      stop = 1;
    }
  }

  int commandsCount = 0;
  command* commandList = getCommands(&commandsCount);

  // Encontre el pipe
  if (stop == 1) {
    if (pipeIndex == 0) {
      _print("El operador pipe no tiene primer operando\n");
      return 0;
    }
    if (pipeIndex == (argCount - 1)) {
      _print("El operador pipe no tiene segundo operando\n");
      return 0;
    }
    int firstCommandIndex = -1;
    int secondCommandIndex = -1;
    char* firstCommand = args[0];
    char* secondCommand = args[pipeIndex + 1];

    for (int i = 0; i < commandsCount && (firstCommandIndex == -1 || secondCommandIndex == -1); i += 1) {
      if (_strcasecmp(firstCommand, commandList[i].name)) {
        firstCommandIndex = i;
      }
      if (_strcasecmp(secondCommand, commandList[i].name)) {
        secondCommandIndex = i;
      }
    }

    if (firstCommandIndex == -1) {
      _fprintf(STDOUT, "%s no es un comando valido\n", firstCommand);
      return 0;
    }
    if (secondCommandIndex == -1) {
      _fprintf(STDOUT, "%s no es un comando valido\n", secondCommand);
      return 0;
    }

    caller callers[2];
    callers[0].argCount = pipeIndex - 1;
    callers[0].args = &(args[1]);
    callers[0].program = commandList[firstCommandIndex];

    callers[1].argCount = argCount - pipeIndex - 2;
    callers[1].args = &(args[pipeIndex + 1]);
    callers[1].program = commandList[secondCommandIndex];

    eventLoop(callers, 2);
    return 1;
  } else {
    int commandIndex = -1;
    for (int i = 0; i < commandsCount && commandIndex == -1; i += 1) {
      if (_strcasecmp(args[0], commandList[i].name)) {
        commandIndex = i;
      }
    }
    
    if (commandIndex == -1) {
      _fprintf(STDOUT, "%s no es un comando valido\n", args[0]);
      return 0;
    }
  
    caller c;
    c.argCount = argCount - 1;
    c.args = &(args[1]);
    c.program = commandList[commandIndex];
    eventLoop(&c, 1);
    return 1;
  }
}

static int init = 1;

void initShell() {
  if (init) {
    clear_screen(1);
    init = 0;
  }
  while (1) {
    sys_showCursor(1);
    _putc(STDOUT, '>');
    char* args[MAX_ARGS];
    int count = getCommandLine(args);
    _putc(STDOUT, '\n');
    sys_showCursor(0);
    runCommandLine(count, args);
  }
}