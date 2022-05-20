
#include <console.h>
#include <syscalls.h>
#include <naiveConsole.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

static void zero_division();
static void invalid_opcode();
static void printRegisters(uint64_t* rsp);


void exceptionDispatcher(int exception, uint64_t rsp) {
	if (exception == ZERO_EXCEPTION_ID){
		zero_division();
	}
	else if(exception == INVALID_OPCODE_ID){
		invalid_opcode();
	}
	printRegisters((uint64_t*) rsp);
	return;
}

static int strlen(char * string){
	if(string == NULL)
		return -1;
	int i = 0;
	while(string[i] != NULL)
		i++;
	return i;
}

static void printRegisters(uint64_t *rsp){
	
	for(int i = 0; i < 16; i++){
		//char buffer[8] = {0};
		ncPrintHex(rsp[i]);
	}
}

static void invalid_opcode(){
	char * msg = "\nException: invalid operand code.\n";
	sys_write(STDERR, msg , strlen(msg));
}

static void zero_division() {
	char * msg = "\nException: division by zero.\n";
	sys_write(STDERR, msg, strlen(msg)); 
}

