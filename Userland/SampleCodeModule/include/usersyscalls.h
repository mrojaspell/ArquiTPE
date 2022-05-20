#ifndef USERSYSCALLS_H
#define USERSYSCALLS_H
#include <stdlib.h>
#include <stdint.h>

enum {
  SYS_READ_ID, 
  SYS_WRITE_ID,
  SYS_CLEAR_ID, 
  SYS_INFOREG_ID, 
  SYS_DNT_ID,
  SYS_PRINTMEM_ID,
  SYS_CURSOR_ID, 
  SYS_SWITCHSCREEN_ID, 
  SYS_TOGGLEMODE_ID
};



extern uint64_t _syscall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

int sys_read(uint8_t fd, char *buffer, size_t count);
int sys_write(uint8_t fd, const char *buffer, size_t count);
int sys_clear(uint8_t fd);
int sys_inforeg(uint64_t *buffer, size_t count);
int sys_dateAndTime(uint64_t id);
int sys_showCursor(int active);
int sys_switchScreen(size_t screen);
int sys_toggleMode(int mode);


#endif