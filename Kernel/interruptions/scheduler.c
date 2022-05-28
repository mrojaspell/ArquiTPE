#include <scheduler.h>
#include <stdint.h>
#include <interrupts.h>
#include <console.h>

const static uint64_t baseRSP = 0x200000;

static uint64_t sampleCodeModuleRSP;

static uint64_t STARTPID = 1000;
#define TASKQUANTITY 16
#define PAGESIZE 1024

static task taskSchedule[TASKQUANTITY] = {{{0}}};
static int currentTask = 0;
static int tasks = 0;
int findNextFreeTask();
int findTask(uint64_t pid);

int nextRunnableTask() {
  // Si vuelve a la misma task, que termine
  for (int i = 0; i < TASKQUANTITY; i += 1) {
    int index = (i + currentTask) % TASKQUANTITY;
    if (taskSchedule[index].status == RUNNING || taskSchedule[index].status == NOTINITIALIZED) {
      return index;
    }
  }
  return currentTask;
}

// Corre la funcion guardandolo en la posicion adecuada del stack. Vuelve a cuando finalmente la funcion termina
void initializeFunction(caller* program, int taskIndex) {
  uint64_t prevRsp = switchRsp(taskIndex * PAGESIZE + baseRSP);
  endInterrupt();
  program->runner(program->argCount, program->args);
  switchRsp(prevRsp);
  return;
} 

uint64_t switchTask(uint64_t rsp) {
  // Si no hay tasks desde el cual cambiar, que continue normalmente
  if (tasks > 0) {
    // Guarda el rsp actual
    taskSchedule[currentTask].rsp = rsp;
    currentTask = nextRunnableTask();
    task* curr = &(taskSchedule[currentTask]);
    switchScreens(curr->program.screenId);
    if (curr->status == NOTINITIALIZED) {
      curr->status = RUNNING;
      initializeFunction(&(curr->program), currentTask);

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

bool loadTask(caller* function, int position, uint64_t parentId) {
  if (tasks == TASKQUANTITY) {
    return false;
  }
  taskSchedule[position].status = NOTINITIALIZED;
  taskSchedule[position].id = STARTPID++;
  taskSchedule[position].program = *function;
  taskSchedule[position].parentId = parentId;
  taskSchedule[position].rsp = 0;
  tasks += 1;
  return true;
}

bool startChild(caller* function) {
  int freeIndex = findNextFreeTask();
  return loadTask(function, freeIndex, (tasks == 0) ? 0 : taskSchedule[currentTask].id);
}

// Comienza una task y para la ejecucion del programa que lo llamo
bool startTask(caller* function, uint64_t rsp) {
  if (tasks > 0) {
    taskSchedule[currentTask].status = PAUSED;
    taskSchedule[currentTask].rsp = rsp;
  } else {
    // Si no hay tasks y se hizo startTask, significa que se llamo desde el sampleCodeModule
    sampleCodeModuleRSP = rsp;
  }
  int freeIndex = findNextFreeTask();
  bool started = loadTask(function, freeIndex, (tasks == 0) ? 0 : taskSchedule[currentTask].id);
  taskSchedule[freeIndex].status = RUNNING;

  // Si el programa es matado antes de hacer sys_exit, nunca vuelve aca. 
  initializeFunction(&(taskSchedule[freeIndex].program), freeIndex);
  return started;
}

bool hasChilds(uint64_t pid) {
  for (int i = 0; i < TASKQUANTITY; i++) {
    if (taskSchedule[i].parentId == pid) {
      return true;
    }
  }
  return false;
}


int findNextFreeTask() {
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
    // Si se mata la funcion que llamo el sampleCodeModule (ej: shell), necesito que vuelva al sampleCodeModule
    forceReturnRsp(sampleCodeModuleRSP);
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