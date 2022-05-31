#include <commands.h>
#include <ustdlib.h>
#include <stdint.h>
#include <RTCID.h>
#include <usersyscalls.h>
#include <math.h>
#include <fibonacci.h>
#include <prime.h>
#include <inforeg.h>

int runOnceProgram() {
  _print("corri una vez\n");

  sys_exit();
  return 1;
}

int runInfiniteProgram() {
  while(1) {
    _print("corro y corro\n");
    _print("vuelo y vuelo\n");
    sys_wait(1);
  }
  sys_exit();
  return 0;
}

static command commands[COMMANDS_LENGTH] = {
  {"test1", &runOnceProgram, "test1", &emptyInit },
  {"test2", &runInfiniteProgram, "test2", &emptyInit },
  { "date&time", &dateAndTime, "Imprime la fecha y el horario.", &emptyInit }, 
  { "divZero", &divZero, "Genera una excepción de dividir por 0", &emptyInit },
  { "fibonacci", &fibonacci, "Imprime infinitamente la sucesion de Fibonacci", &emptyInit },
  { "hello", &holaMundo, "Saluda al mundo.", &emptyInit }, 
  { "help", &help, "Muestra una lista de los comandos.", &emptyInit }, 
  { "inforeg", &infoReg, "Imprime los registros con sus valores al llamar esta función.", &emptyInit },
  { "invalidOpcode", &invalidOpcode, "Genera una excepción de operador invalido", &emptyInit },
  { "prime", &primes, "Imprime infinitamente numeros primos", &emptyInit},
  { "printmem", &printmem, "Volcado de memoria de 32 bytes a partir de la dirección recibida como  argumento.", &emptyInit},
};

void emptyInit(int screenId){
  return;
}

int wait() {
  sys_wait(1);
  sys_exit();
  return 1;
}


int divZero(){
  int aux1 = 1;
  int aux2 = 0;
  int div0 = aux1/aux2;
  return div0;
}

int invalidOpcode(){
  _opcodeExp();
  sys_exit();
  return 1;
}

int printmem(int argc, char* argv[]){
  if (argc < 1) {
    _print("No se dio una direccion de memoria\n");
    return 1;
  } 
  char* address = argv[0];
  

  // Chequear que strToHex devuelva -1 si no es hex
  uint64_t memDir = strToHex(address);
	if(memDir == -1 || memDir >= LAST_MEM){
    _fprintf(STDOUT,"\nLa direccion ingresada no es alcanzable (%s)\n",address);
    return 1;
  }
  _fprintf(STDOUT, "\nDump de 32 bytes a partir de la direccion: %s\n\n", address);

  uint8_t buffer[DUMP_SIZE];
  sys_printMem(memDir, buffer, DUMP_SIZE);
  for(int i = 0; i < DUMP_SIZE; i++){
      if(i % 8==0){
        _fprintf(STDOUT,"\n x%d:  ",i);
      }
      _fprintf(STDOUT, "0x%x  ", buffer[i]);
  }
  _fprintf(STDOUT,"\n");
  sys_exit();
  return 1;
}

int dateAndTime(){
  uint64_t date = sys_dateAndTime(DAY_RTC_ID);
  uint64_t month = sys_dateAndTime(MONTH_RTC_ID);
  uint64_t year = sys_dateAndTime(YEAR_RTC_ID);
  _fprintf(STDOUT, "La fecha de hoy es: %d/%d/%d", date, month, year);
  uint64_t hour = sys_dateAndTime(HOUR_RTC_ID);
  uint64_t minute = sys_dateAndTime(MINUTE_RTC_ID);
  uint64_t second = sys_dateAndTime(SECOND_RTC_ID);
  _fprintf(STDOUT,"\n Y el horario de este momento es: %d:%d:%d\n", hour, minute, second);
  sys_exit();
  return 1;
}

int help(){
  _fprint(STDOUT, "Lista de posibles comandos: \n");
  for (uint8_t i = 0; i < COMMANDS_LENGTH; i++){
    _fprintf(STDOUT, "\t%s\n", commands[i].name);
  }

  _print("Lista de posibles operaciones: \n");
  _print("\tprogram1 | program2\n");
  sys_exit();
  return 1;
}

int infoReg(){
  static uint64_t registers[TOTAL_REGISTERS];
  sys_inforeg(registers);

  for(int i = 0 ; i < TOTAL_REGISTERS; i++){
    _fprintf(STDOUT, "%s",registerNames[i]);
    _fprintf(STDOUT, "%x\n",registers[i]);
  }
  sys_exit();
  return 1;
}

int holaMundo() {
  _fprintf(STDOUT, "hola mundo");
  sys_exit();
  return 1;
}

command* getCommands() {
  return commands;
}