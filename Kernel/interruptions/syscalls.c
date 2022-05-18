#include <syscalls.h>
#include <keyboard.h>
#include <registerNames.h>
#include <console.h>

static uint64_t registers[16] = {0};  // comienzan en 0 por default (mas entendible que no tienen nada guardado)



uint8_t getCurrentTime(uint64_t rtcID){
	uint8_t x = _getRTCInfo(rtcID);
	uint8_t result = ((x / 16) * 10) + (x & 0xf);
	return result;
}

void getRegisterData(uint64_t* rsp){
    for(int i =0 ; i < REGISTERS ; i++){
        registers[i] = rsp[i];
    }
}

uint64_t* infoReg(){
    return registers;
}

void printMem(uint64_t direc, uint8_t * buffer, uint64_t bytes){
    for (uint8_t i = 0; i < bytes; i++) {
        buffer[i] = (uint8_t) _getMem(direc+i);     // llamo 'bytes'(32) veces a _getMem con 32 posiciones de memoria distintos
    }
}

void syscallHandler(uint8_t rax, void* arg0, void* arg1, void* arg2) {
    switch (rax) {
        case WRITE:
            print((uint8_t)arg0, (char *)arg1, (size_t)arg2);
        break;
    }
    return;
}

uint64_t sys_read(){
    getChar();
    return 0;
}

/*void * int_80(uint8_t argc, void * argv[] ) { }*/