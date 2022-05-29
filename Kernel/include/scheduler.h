#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>
typedef enum status {
  NOTPRESENT, RUNNING, PAUSED, NOTINITIALIZED
} status;

typedef struct caller {
  int (*runner)(unsigned int count, void** args);
  void** args;
  unsigned int argCount;
  unsigned int screenId;
} caller;

typedef struct task {
  caller program;
  uint64_t rsp;
  uint64_t parentId;
  status status;
  uint64_t id;
} task;

uint64_t switchTask(uint64_t rsp);
bool startTask(caller* function, uint64_t rsp);
bool startChild(caller* function);
bool killTask(uint64_t pid);
bool pauseTask(uint64_t pid);
int resumeTask(uint64_t pid);
uint64_t getPid();
bool currentHasChilds();
uint64_t forceSwitchTask();

/*
Switch task(rsp)
Restore parent
Check if there are childs 
Next task
Start task
Save Task
Pause Task
Resume Task
Kill task(pid)
Find Task
*/


#endif