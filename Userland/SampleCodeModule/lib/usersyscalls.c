#include <usersyscalls.h>

int sys_read(uint8_t fd, char *buffer, size_t count) {
  return _syscall(SYS_READ_ID, fd, (uint64_t)buffer, 1);
}

int sys_write(uint8_t fd, const char *buffer, size_t count) {
    return _syscall(SYS_WRITE_ID, fd, (uint64_t)buffer, count);
}

int sys_clear(uint8_t fd) {
    return _syscall(SYS_CLEAR_ID, fd, 0, 0);
}

int sys_inforeg(uint64_t *buffer, size_t count){
  return _syscall(SYS_INFOREG_ID, 0, (uint64_t)buffer, count);
}

int sys_dateAndTime(uint64_t id){
  return _syscall(SYS_DNT_ID, (uint64_t)id, 0, 0);
}