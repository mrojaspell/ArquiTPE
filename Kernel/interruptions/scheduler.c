#include <scheduler.h>
#include <stdint.h>
#include <interrupts.h>
#include <console.h>

const static uint64_t baseRSP = 0x200000;

static uint64_t STARTPID = 1000;
#define TASKQUANTITY 16
#define PAGESIZE 1024

static task taskSchedule[TASKQUANTITY] = {{{0}}};
static int currentTask = 0;
static int tasks = 0;
int findFreeTask();
int findTask(uint64_t pid);

int nextRunnableTask() {
  // Si vuelve a la misma task, que termine
  for (int i = (currentTask + 1) % TASKQUANTITY; i != currentTask; i = (i + 1) % TASKQUANTITY) {
    if (taskSchedule[i].status == RUNNING || taskSchedule[i].status == NOTINITIALIZED) {
      return i;
    }
  }
  return currentTask;
}

uint64_t switchTask(uint64_t rsp) {
  if (tasks > 0) {
    // Saves current rsp
    taskSchedule[currentTask].rsp = rsp;
    currentTask = nextRunnableTask();
    task* curr = &(taskSchedule[currentTask]);
    switchScreens(curr->program.screenId);
    if (curr->status == NOTINITIALIZED) {
      curr->status = RUNNING;
      endInterrupt();
      // Guarda la funcion en su correspondiente lugar de stack
      uint64_t prevRsp = switchRsp(currentTask * PAGESIZE + baseRSP);
      curr->program.runner(curr->program.argCount, curr->program.args);
      switchRsp(prevRsp);

      // Si llego aca, entonces termino la funcion de correr, y quiero que corra la siguiente task
      return switchTask(rsp);
    }
    return curr->rsp;
  }
  return rsp;
}

uint64_t getPid() {
  if (tasks > 0) {
    return taskSchedule[currentTask].id;
  }
  return 0;
}

bool startChild(caller* function) {
  int freeIndex = findFreeTask();
  if (freeIndex == -1) return false;
  taskSchedule[freeIndex].status = NOTINITIALIZED;
  taskSchedule[freeIndex].id = STARTPID++;
  taskSchedule[freeIndex].program = *function;
  taskSchedule[freeIndex].parentId = (tasks == 0) ? 0 : taskSchedule[currentTask].id;
  taskSchedule[freeIndex].rsp = 0;
  tasks += 1;
  return true;
}

bool startTask(caller* function) {
  if (tasks > 0) {
    taskSchedule[currentTask].status = PAUSED;
  }

  // QUE PUEDE PASAR ACAAA
  return startChild(function);
}

bool hasChilds(uint64_t pid) {
  for (int i = 0; i < TASKQUANTITY; i++) {
    if (taskSchedule[i].parentId == pid) {
      return true;
    }
  }
  return false;
}


int findFreeTask() {
  for (int i = 0; i < TASKQUANTITY; i++) {
    if (taskSchedule[i].status == NOTPRESENT) return i;
  }
  return -1;
}

// Mata recursivamente una task. Toda task terminada tiene que terminar sus hijos
// Devuelve verdadero si fue terminado, falso si no
// Se fija que la task padre si tiene hijos o no. Si no tiene hijos, resumirlo.
bool killTask(uint64_t pid) {
  int index = findTask(pid);
  if (index == -1 || taskSchedule[index].status == NOTPRESENT) return false;
  for (int i = 0; i < TASKQUANTITY; i++) {
    if (taskSchedule[i].parentId == pid) {
      killTask(taskSchedule[i].id);
    }
  }
  tasks -= 1;
  taskSchedule[index].status = NOTPRESENT;

  if (taskSchedule[index].parentId == 0) {
    // Programar task null que haga while (1); -> call syscall exit
    return true;
  }
  int parentIndex = findTask(taskSchedule[index].parentId);
  if (!hasChilds(taskSchedule[index].parentId)) {
    taskSchedule[parentIndex].status = RUNNING;
  }

  return true;
}

int findTask(uint64_t pid){
    for(int i = 0; i < TASKQUANTITY; i++){
        if(taskSchedule[i].id == pid && taskSchedule[i].status != NOTINITIALIZED)
            return i;
    }
    return -1;
}

bool pauseTask(uint64_t pid){
  int index = findTask(pid);
  if (index == -1){
    return false;
  }
  taskSchedule[index].status = PAUSED;
  return true; 
}

int resumeTask(uint64_t pid ){
  int index = findTask(pid);
  if(index == -1){
    return false;
  }
  taskSchedule[index].status = RUNNING;
  return true;
}