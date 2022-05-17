#include <time.h>
#include <keyboard.h>
#include <stdint.h>
#include <naiveConsole.h>

#define KEYBOARD 1
#define TIMER 0


static void int_20();
static void int_21();


void irqDispatcher(uint64_t irq, uint64_t rsp) {
	switch (irq) {
		case TIMER:
			int_20();
			break;
		case KEYBOARD:
			int_21(rsp);
			break;
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_21(uint64_t rsp){
	keyboardHandler(rsp);
}
