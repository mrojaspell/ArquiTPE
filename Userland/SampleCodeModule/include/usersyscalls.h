#ifndef USERSYSCALLS_H
#define USERSYSCALLS_H
#include <stdlib.h>
#include <stdint.h>

extern uint64_t _syscall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

int sys_read(uint8_t fd, char *buffer, size_t count);
int sys_write(uint8_t fd, const char *buffer, size_t count);
int sys_clear(uint8_t fd);
int sys_inforeg(uint64_t *buffer, size_t count);

#endif