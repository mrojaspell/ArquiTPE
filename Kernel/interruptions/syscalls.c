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

void syscallHandler(uint8_t rax, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
    switch (rax) {
        case WRITE: 
            for (int i = 0; i < (size_t)arg2; i += 1) {
                if (arg0 == STDOUT) {
                    ncPrintChar(((char *)arg1)[i]);
                } else {
                    ncPrintCharColor(((char *)arg1)[i], RED);
                }
            }
        break;
    }
    return;
}


/*void * int_80(uint8_t argc, void * argv[] ) { }*/