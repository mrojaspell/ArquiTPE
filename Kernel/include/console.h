#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>
#include <stdlib.h>
#include <colors.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

typedef struct {
	uint8_t x;
	uint8_t y;
} vec;

typedef struct {
    uint8_t width;
    uint8_t height;
    vec start;
    vec currPos;
} window;


typedef enum {
  STDIN = 0, STDOUT, STDERR, STDLEFT, STDRIGHT
} FILE_DESCRIPTOR;


void loadWindows();
void newLine(FILE_DESCRIPTOR fd);
void printCharColor(FILE_DESCRIPTOR fd, char c, color_t charColor, color_t bgColor, int next);
void printChar(FILE_DESCRIPTOR fd, char c);
void clearScreen(FILE_DESCRIPTOR fd);
void scrollUp(FILE_DESCRIPTOR fd);
void print(FILE_DESCRIPTOR fd, char* str, size_t count);
void initializeDualScreen();
void showCursor(FILE_DESCRIPTOR fd);
void stopCursor(FILE_DESCRIPTOR fd);

#endif

