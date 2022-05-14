#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>

#define WRITE 0x01
uint8_t getCurrentTime(uint64_t rtcID);
uint64_t* getRegisters();

#endif