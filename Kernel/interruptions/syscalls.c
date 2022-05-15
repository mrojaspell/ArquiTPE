#include <syscalls.h>
#include <keyboard.h>
#include <registerNames.h>
#include <console.h>


// static uint64_t regs[16] = {0};


uint8_t getCurrentTime(uint64_t rtcID){
	uint8_t x = getRTCInfo(rtcID);
	uint8_t result = ((x / 16) * 10) + (x & 0xf);
	return result;
}

uint64_t* getRegisters(){
    // return registers;
    return 0; //??
}


void syscallHandler(uint8_t rax, void* arg0, void* arg1, void* arg2) {
    switch (rax) {
        case WRITE:
            sys_write((FILE_DESCRIPTOR)arg0, (char *)arg1, (size_t)arg2);
        break;
    }
    return;
}

void sys_write(FILE_DESCRIPTOR fd, char* string, size_t count) {
    print(fd, string, count);
}

uint64_t sys_read(){
    getChar();
}

/*void * int_80(uint8_t argc, void * argv[] ) { }*/