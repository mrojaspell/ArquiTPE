#ifndef USTDLIB_H
#define USTDLIB_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

enum {
  STDIN = 0, STDOUT, STDERR, STDLEFT, STDRIGHT
};

#define IS_DIGIT(x) (((x) >= '0' && (x) <= '9'))
void clear_screen(unsigned int fd);
void fprint(uint8_t fd, const char *str);
int getChar();
void _putc(uint8_t fd, char c);
void _fprintf(uint8_t fd, char *,...);              
int toLower(int chr);

#endif