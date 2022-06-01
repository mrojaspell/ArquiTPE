#ifndef COMMANDS_H
#define COMMANDS_H
#define COMMANDS_LENGTH 10
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
} command;

command* getCommands();

/*
 Lista de comandos y su estructura
 command {
   char* name;        NOMBRE DE LA FUNCION
   void (*runner)();  FUNCION A CORRER
 } 
*/

static char* commandInfo[COMMANDS_LENGTH + 1] = { 
                          "date&time : Imprime en patalla la fecha del ano corriente y horario en que fue\nllamado.\n",
                          "divzero: Realiza una division por 0. Lanza una excepcion e imprime una captura de los registros al momento de ejecucion.\n",
                          "fibonacci: Imprime la secuencia de fibonacci infinitamente hasta que se pause o se termine su ejecucion.\n",
                          "hello: Imprime un saludo al usuario.\n",
                          "help: Imprime una lista detallada de los comandos  y modulos ejecutables del\nprograma.\n",
                          "inforeg: Imprime una captura de los registros al momento de ser ejecutada la\nfuncion.\n",
                          "invalidOpcode: Lanza la excepcion de invalid operand code e imprime los\nregistros al momento de ejecucion.\n",
                          "prime: imprime numeros primos infinitamente hasta que se pause o se termine su\nejecucion.\n",
                          "printmem: Recibe como argumento una direccion de memoria no superior a\n80000000h y luego imprime los proximos 32bytes de memoria adyacentes a la\ndireccion dada.\n",
                          "clear: Limpia la pantalla\n",
                          "operaciones: usando la operacion | se puede correr dos funciones al mismo\ntiempo. Para terminar la ejecucion de un programa: \"esc\". Para pausar un\nprograma: \"p\", resumir: \"r\". Mientras se usa la operacion \"|\", pausar y resumir\nfuncion en pantalla izquierda \"a\", derecha \"a\".\n",
};

extern int _opcodeExp();

int dateAndTime();
int help();
int infoReg();
int holaMundo();
int printmem(int argc, char* argv[]);
int divZero();
int invalidOpcode();
int fibonacci();
int primes();
void emptyInit(int screenId);
int wait();
int getDescriptions(char* function);
int clearScreen();

#endif