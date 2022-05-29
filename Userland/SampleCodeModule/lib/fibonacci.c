#include <ustdlib.h>
#include <fibonacci.h>
#include <usersyscalls.h>

int fibonacci(){
  int a = 0;
  int b = 1;
  int c;
  int count = 2;
  _fprintf(STDOUT, "n %d: %u\n", 1, 0);
  sys_wait(1);
  _fprintf(STDOUT, "n %d: %u\n", 2, 1);
  while(1){
    c = a + b;
    a = b;
    b = c;
    count++;
    sys_wait(1);
    _fprintf(STDOUT,"n %d: %u\n",count ,c);
  }
  sys_exit();
  return c;
}
