#include <scheduler.h>
#include <stdint.h>
#include <interrupts.h>

const static uint64_t baseRSP = 0x200000;

static uint64_t STARTPID = 1000;
#define TASKQUANTITY 16
#define PAGESIZE 1024

static task taskSchedule[TASKQUANTITY] = {{{0}}};
static int currentTask = 0;
static int tasks = 0;
int findFreeTask();

void nextTask() {
  if (tasks != 0) {
    currentTask = (currentTask + 1) % TASKQUANTITY;
    while (taskSchedule[currentTask].status == NOTPRESENT || taskSchedule[currentTask].status == PAUSED) {
      currentTask += 1;
    }
  } 
}

uint64_t switchTask(uint64_t rsp) {
  taskSchedule[currentTask].rsp = rsp;
  nextTask();
  task* curr = &(taskSchedule[currentTask]);
  if (curr->status == NOTINITIALIZED) {
    curr->status = RUNNING;
    endInterrupt();
    switchRsp(currentTask * PAGESIZE + baseRSP); // Guarda la funcion en su correspondiente lugar de stack
    curr->program.runner(curr->program.argCount, curr->program.args);
    return -1; // Nunca va a correr
  }
  return curr->rsp;
}

bool hasChilds(uint64_t pid) {
  for (int i = 0; i < TASKQUANTITY; i++) {
    if (taskSchedule[i].parentId == pid) {
      return true;
    }
  }
  return false;
}

bool startTask(caller function) {
  int freeIndex = findFreeTask();
  if (freeIndex == -1) return false;
  taskSchedule[freeIndex].status = NOTINITIALIZED;
  taskSchedule[freeIndex].id = STARTPID++;
  return true;
}

int findFreeTask() {
  for (int i = 0; i < TASKQUANTITY; i++) {
    if (taskSchedule[i].status == NOTPRESENT) return i;
  }
  return -1;
}

int findTask(uint64_t pid){
    for(int i = 0; i < TASKQUANTITY; i++){
        if(taskSchedule[i].id == pid && taskSchedule[i].status != NOTINITIALIZED)
            return i;
    }
    return -1;
}