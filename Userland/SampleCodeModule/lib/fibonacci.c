#include <ustdlib.h>
#include <fibonacci.h>

static fiboS fiboState[2] = {{1, 1, 0}, {1, 1, 0}};

void initFibonacci(int screenId){
  fiboState[screenId].n_2 = 1;
  fiboState[screenId].n_1 = 1;
  fiboState[screenId].cantPrinted = 0;
}

//anda bien (solucionar overflow)
int fibonacci(int argc, char* argv[], int screenId){

  //sleep casero para debuggear
  for (size_t i = 0; i < 50000000; i++)
  {
    /* code */
  }

  uint64_t toPrint;
  if(fiboState[screenId].cantPrinted == 0)
    toPrint = 1;
  else if(fiboState[screenId].cantPrinted == 1)
    toPrint = 1;
  else{
    toPrint = fiboState[screenId].n_2 + fiboState[screenId].n_1;
    fiboState[screenId].n_2 = fiboState[screenId].n_1;
    fiboState[screenId].n_1 = toPrint;
  }

  //para debugging
  if(fiboState[screenId].n_2 > fiboState[screenId].n_1)
    _fprintf(STDOUT, "OVERFLOW\n\n\n");

  _fprintf(STDOUT, "El fibo nro %d es %u\n", ++(fiboState[screenId].cantPrinted), toPrint); //fijarse que fprintf imprima nros de 64bit
  return 0;
}
