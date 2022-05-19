#include <ustdlib.h>
#include <shell.h>
#include <ctype.h>
#include <usersyscalls.h>
#include <RTCID.h>

#define BUFFER_LENGTH 256
#define COMMANDS_LENGTH 3


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

//ponerlos en orden alfabetico plssssssss
static command commands[COMMANDS_LENGTH] = {{"date&time", &dateAndTime}, { "hello", &holaMundo }, { "help", &help }, { "inforeg", &infoReg }};


void dateAndTime(){
  _fprint(STDOUT, "La fecha de hoy es: ");
  uint64_t date = sys_dateAndTime(DAY_RTC_ID);    // por lo que entendi estamos haciendo que las syscalls solo llamen a la sys call por lo que implemento todo los prints aca
  uint64_t month = sys_dateAndTime(MONTH_RTC_ID);    // si no es asi se puede cambiar rapidamente
  uint64_t year = sys_dateAndTime(YEAR_RTC_ID);
  _fprint(STDOUT, date); _fprint(STDOUT, month); _fprint(STDOUT, year); // corregir cuando se implemente correctamente printf
  _fprint(STDOUT,"\n Y el horario de este momento es: "); // esto es solo un modelo 
  uint64_t hour = sys_dateAndTime(HOUR_RTC_ID);
  uint64_t minute = sys_dateAndTime(MINUTE_RTC_ID);
  uint64_t second = sys_dateAndTime(SECOND_RTC_ID);
  // print la hora minuto segundo hasta que no implementewmos bien los prints no lo voy a escribir
}

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
  static uint64_t registers[16]; //despues arreglar tamaño
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