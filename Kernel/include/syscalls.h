#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>
#include <stdlib.h>

typedef enum {
  READ=0, WRITE, CLEAN_SCREEN
} syscall_id;

#define REGISTERS 16

extern uint8_t _getRTCInfo(uint64_t);
uint8_t getCurrentTime(uint64_t rtcID);
uint64_t* infoReg();
void getRegisterData(uint64_t* rsp);
void printMem(uint64_t direc, uint8_t * buffer, uint64_t bytes);
extern uint8_t _getMem(uint64_t direc);


#endif