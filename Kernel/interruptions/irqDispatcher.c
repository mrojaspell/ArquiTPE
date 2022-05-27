#include <time.h>
#include <keyboard.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <scheduler.h>

#define KEYBOARD 1
#define TIMER 0

uint64_t irqDispatcher(uint64_t irq, uint64_t rsp) {
	switch (irq) {
		case TIMER:
			timer_handler();
			rsp = switchTask(rsp);
			break;
		case KEYBOARD:
			keyboardHandler();
			break;
	}
	return rsp;
}