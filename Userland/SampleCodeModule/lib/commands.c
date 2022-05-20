#include <commands.h>
#include <ustdlib.h>
#include <stdint.h>
#include <RTCID.h>
#include <usersyscalls.h>

static command commands[COMMANDS_LENGTH] = {
  { "date&time", &dateAndTime, "Imprime la fecha y el horario."}, 
  { "divZero", &divZero, "Genera una excepción de dividir por 0"},
  { "hello", &holaMundo, "Saluda al mundo." }, 
  { "help", &help, "Muestra una lista de los comandos." }, 
  { "inforeg", &infoReg, "Imprime los registros con sus valores al llamar esta función." },
  { "invalidOpcode", &invalidOpcode, "Genera una excepción de operador invalido"},
  { "printmem", &printmem, "Volcado de memoria de 32 bytes a partir de la dirección recibida como  argumento."},
};

void divZero(){
  int aux1 = 1;
  int aux2 = 0;
  int div0 = aux1/aux2;
  return;
}

void invalidOpcode(){
  _opcodeExp();
}

void printmem(char* address){
  uint64_t memDir = strToHex(address);
	if(memDir == -1){
    _fprintf(STDOUT,"\nLa direccion ingresada no es alcanzable (%s)",address);
    return;
  }
  _fprintf("\nDump de 32 bytes a partir de la direccion: %s\n\n", address);
  uint8_t buffer[DUMP_SIZE];
  sys_printMem(memDir, buffer, DUMP_SIZE);
  for(int i = 0; i < DUMP_SIZE; i++){
      if(i == 16){
        _fprintf(STDOUT,"\n");
      }
      
      _fprintf(STDOUT, "%d", buffer[i]);
  }
  _fprintf(STDOUT,"\n");
  return;
}

void dateAndTime(){
  uint64_t date = sys_dateAndTime(DAY_RTC_ID);
  uint64_t month = sys_dateAndTime(MONTH_RTC_ID);
  uint64_t year = sys_dateAndTime(YEAR_RTC_ID);
  _fprintf(STDOUT, "La fecha de hoy es: %d/%d/%d", date, month, year);
  uint64_t hour = sys_dateAndTime(HOUR_RTC_ID);
  uint64_t minute = sys_dateAndTime(MINUTE_RTC_ID);
  uint64_t second = sys_dateAndTime(SECOND_RTC_ID);
  _fprintf(STDOUT,"\n Y el horario de este momento es: %d:%d:%d", hour, minute, second);
}

void help(){
  _fprint(STDOUT, "Lista de posibles comandos: \n");
  for (uint8_t i = 0; i < COMMANDS_LENGTH; i++){
    _fprintf(STDOUT, "\t%s\n", commands[i].name);
  }
}

void infoReg(){
  static uint64_t registers[16]; //despues arreglar tamaño
  sys_inforeg(registers, (sizeof(registers)/sizeof(registers[0])));
}

void holaMundo() {
  _fprintf(STDOUT, "hola mundos");
  return;
}

command* getCommands(int* size) {
  *size = COMMANDS_LENGTH;
  return commands;
}