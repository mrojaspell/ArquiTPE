#include <syscalls.h>
#include <keyboard.h>
#include <registerNames.h>
#include <console.h>

static uint64_t regs[16] = {0};

uint8_t getCurrentTime(uint64_t rtcID){
	uint8_t x = _getRTCInfo(rtcID);
	uint8_t result = ((x / 16) * 10) + (x & 0xf);
	return result;
}

uint64_t* getRegisters(){
    return registers;
}

//todo fede hacete esto xddddddd
void * int_80(uint8_t fd, void * arg1, void * arg2, void * arg3, void * arg4){
    
}


/*void * int_80(uint8_t argc, void * argv[] ) { }*/