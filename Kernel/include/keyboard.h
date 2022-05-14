#ifndef KEYBOARD_DRIVER
#define KEYBOARD_DRIVER

#include <stdint.h>
#include <naiveConsole.h>
#include <lib.h>
#include <colors.h>

//busca el numero de caracter en mi array y lo imprime
void printKey(uint8_t key);

void printKeyMayus(uint8_t key);

void keyboard_handler();

#endif