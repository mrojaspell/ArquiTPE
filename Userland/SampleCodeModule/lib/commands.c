#include <commands.h>
#include <ustdlib.h>
#include <stdint.h>
#include <RTCID.h>
#include <usersyscalls.h>
#include <math.h>
#include <fibonacci.h>
#include <prime.h>
#include <inforeg.h>


static command commands[COMMANDS_LENGTH] = {
  { "date&time", &dateAndTime }, 
  { "divZero", &divZero },
  { "fibonacci", &fibonacci },
  { "hello", &holaMundo }, 
  { "help", &help }, 
  { "inforeg", &infoReg },
  { "invalidOpcode", &invalidOpcode },
  { "prime", &primes },
  { "printmem", &printmem },
  { "clear", &clearScreen }
};


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
    char* error_message = "No se dio una direccion de memoria\n";
    sys_write(STDERR, error_message, _strlen(error_message));
    sys_exit();
    return 1;
  } 
  char* address = argv[0];
  

  // Chequear que strToHex devuelva -1 si no es hex
  uint64_t memDir = strToHex(address);
	if(memDir == -1 || memDir >= LAST_MEM){
    char* error_message = "\nLa direccion ingresada no es alcanzable \n";
    sys_write(STDERR, error_message,_strlen(error_message));
    sys_exit();
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

int help(int argc, char* argv[]){
  if(argc == 0){
    _fprint(STDOUT, "Lista de posibles comandos: \n");
    for (uint8_t i = 0; i < COMMANDS_LENGTH; i++){
      _fprintf(STDOUT, "\t%s\n", commands[i].name);
    }
    _print("Lista de posibles operaciones: \n");
    _print("\tprogram1 | program2\n");
    _print("help \"comando\"  para desplegar informacion detallada de cada comando\nhelp operaciones para informacion acerca de las operaciones posibles.\n");
  } else {
    int index = getDescriptions(argv[0]);
    if (index != -1) {
      char* mensaje = commandInfo[index];
      _print(mensaje);
    } else {
      _fprintf(STDOUT, "%s no existe\n", argv[0]);
    }
  }
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

int getDescriptions(char* function){
  int command = -1;
  
  for(int i = 0 ; i < COMMANDS_LENGTH; i++){
    if(_strcasecmp(function, commands[i].name)){
        command = i;
    }
  }
  if(_strcasecmp(function,"operaciones")){
    command = COMMANDS_LENGTH;
  }
  return command;
}

int clearScreen() {
  clear_screen(0);
  sys_exit();
}