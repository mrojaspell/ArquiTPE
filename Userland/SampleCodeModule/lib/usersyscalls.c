#include <usersyscalls.h>

int sys_read(uint8_t fd, char *buffer, size_t count) {
  return _syscall(0, fd, (uint64_t)buffer, 1);
}

int sys_write(uint8_t fd, const char *buffer, size_t count) {
    return _syscall(1, fd, buffer, count);
}

int sys_clear(uint8_t fd) {
    return _syscall(2, fd, 0, 0);
}