#include <syscalls.h>
#include <keyboard.h>
#include <registerNames.h>
#include <console.h>
#include <naiveConsole.h>

//creo que esta al pedo, si funciona todo lo borro
//static uint64_t registers[REGISTERS] = {0};  // comienzan en 0 por default (mas entendible que no tienen nada guardado)

uint8_t sys_dateAndTime(uint64_t rtcID){
	uint8_t x = _getRTCInfo(rtcID);
	uint8_t result = ((x / 16) * 10) + (x & 0xf);
	return result;
}

int sys_inforeg(uint64_t *buffer, size_t count){
    if(buffer == NULL || count < REGISTERS*sizeof(uint64_t))
        return -1;

    for(int i = 0 ; i < REGISTERS ; i++){
        //buffer[i] = rsp[i];
    }

    return 0;
}

void sys_getMem(uint64_t direc, uint8_t * buffer, uint64_t bytes){
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
        printCharColor(buffer[i], fontColor, BLACK, 1);
    }
    return i;
}

int syscallHandler(syscall_id rax, void* arg0, void* arg1, void* arg2) {
    switch (rax) {
        case SYS_READ:
            return sys_read((FILE_DESCRIPTOR)arg0, (char *)arg1, (size_t)arg2);
            break;
        case SYS_WRITE:
            return sys_write((FILE_DESCRIPTOR)arg0, (char *)arg1, (size_t)arg2);
            break;
        case SYS_CLEAN_SCREEN:
            clearScreen((FILE_DESCRIPTOR)arg0);
            return 0;
            break;
        case SYS_INFOREG:
            return sys_inforeg((uint64_t *)arg1, (size_t) arg2);
            break;
        case SYS_DATENTIME:
            return sys_dateAndTime((uint64_t) arg0);
            break;
        case SYS_PRINTMEM:
            sys_getMem((uint64_t)arg0, (uint8_t *)arg1, (uint64_t) arg2);
            return 0;
            break;
        case SYS_SET_CURSOR:
            setCursor((int)arg0);
            return 0;
            break;
        case SYS_SET_SCREEN:
            switchScreens((size_t)arg0);
            return 0;
            break;
        case SYS_TOGGLE_MODE:
            if (!(size_t)arg0) {
                initializeSingleScreen();
            } else {
                initializeDualScreen();
            }
            return 0;
            break;
    }
    return -1;
}
