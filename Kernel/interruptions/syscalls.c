#include <syscalls.h>
#include <keyboard.h>
#include <registerNames.h>
#include <console.h>
#include <naiveConsole.h>

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


int sys_read(FILE_DESCRIPTOR fd, char* buffer, size_t count){
    if (buffer == 0 || count == 0 || fd != 0) return -1;

    uint8_t i = 0;
    int c;
    while (i < count && (c = getChar()) != -1) {
        buffer[i++] = c;
    }
    return i;
}

int sys_write(FILE_DESCRIPTOR fd, const char* buffer, uint64_t size) {
    if (buffer == 0 || size == 0 || fd > 4) {
        return -1;
    }

    color_t fontColor = (fd == STDERR) ? (RED) : (WHITE);
    int i;
    for (i = 0; i < size && buffer[i]; i += 1) {
        printCharColor(fd, buffer[i], fontColor, BLACK, 1);
    }
    return i;
}

int syscallHandler(syscall_id rax, void* arg0, void* arg1, void* arg2) {
    switch (rax) {
        case READ:
            return sys_read((FILE_DESCRIPTOR)arg0, (char *)arg1, (size_t)arg2);
        case WRITE:
            return sys_write((FILE_DESCRIPTOR)arg0, (char *)arg1, (size_t)arg2);
        case CLEAN_SCREEN:
            clearScreen((FILE_DESCRIPTOR)arg0);
            return 0;
    }
    return -1;
}
