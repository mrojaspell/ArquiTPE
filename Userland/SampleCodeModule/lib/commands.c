#include <commands.h>
#include <ustdlib.h>
#include <stdint.h>
#include <RTCID.h>
#include <usersyscalls.h>

static command commands[COMMANDS_LENGTH] = {
  {"date&time", &dateAndTime, "Imprime la fecha y el horario"}, 
  { "hello", &holaMundo, "Saluda al mundo" }, 
  { "help", &help, "Muestra una lista de los comandos" }, 
  { "inforeg", &infoReg, "Imprime los registros" }
};

void dateAndTime(){
  uint64_t date = sys_dateAndTime(DAY_RTC_ID);    // por lo que entendi estamos haciendo que las syscalls solo llamen a la sys call por lo que implemento todo los prints aca
  uint64_t month = sys_dateAndTime(MONTH_RTC_ID);    // si no es asi se puede cambiar rapidamente
  uint64_t year = sys_dateAndTime(YEAR_RTC_ID);
  _fprintf(STDOUT, "La fecha de hoy es: %d/%d/%d", date, month, year);
  uint64_t hour = sys_dateAndTime(HOUR_RTC_ID);
  uint64_t minute = sys_dateAndTime(MINUTE_RTC_ID);
  uint64_t second = sys_dateAndTime(SECOND_RTC_ID);
  _fprintf(STDOUT,"\n Y el horario de este momento es: %d:%d:%d", hour, minute, second); // esto es solo un modelo 
}

void help(){
  _fprint(STDOUT, "Lista de posibles comandos: \n");
  for (uint8_t i = 0; i < COMMANDS_LENGTH; i++){
    _fprintf(STDOUT, "\t%s\n", commands[i].name); //CORREGIR fprintf
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