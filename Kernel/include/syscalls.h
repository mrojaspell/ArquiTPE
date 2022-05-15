#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>
#include <stdlib.h>

#define WRITE 0x01
uint8_t getCurrentTime(uint64_t rtcID);
extern uint8_t getRTCInfo(uint64_t);

uint64_t* getRegisters();
void sys_write(FILE_DESCRIPTOR fd, char* string, size_t count);
uint64_t sys_read();


#endif