#include <ustdlib.h>
#include <prime.h>
#include <usersyscalls.h>
/*
// Function to find the square-root of N
// fijarse si conseguimos una mas eficiente
int _sqrt(int number){
    int start = 0, end = number;
    int mid;
 
    // To store the answer
    float ans;
 
    // To find integral part of square
    // root of number
    while (start <= end) {
 
        // Find mid
        mid = (start + end) / 2;
 
        // If number is perfect square
        // then break
        if (mid * mid == number) {
            ans = mid;
            break;
        }
 
        // Increment start if integral
        // part lies on right side
        // of the mid
        if (mid * mid < number) {
          //first start value should be added to answer
            ans=start;
          //then start should be changed
            start = mid + 1;
        }
 
        // Decrement end if integral part
        // lies on the left side of the mid
        else {
            end = mid - 1;
        }
    }
 
    // To find the fractional part
    // of square root upto 5 decimal
    float increment = 0.1;
    for (int i = 0; i < 5; i++) {
        while (ans * ans <= number) {
            ans += increment;
        }
 
        // Loop terminates,
        // when ans * ans > number
        ans = ans - increment;
        increment = increment / 10;
    }
    return (int)(ans+0.5);
}

uint8_t isPrime(uint64_t num){
  float numsqrt = _sqrt(num);
  int isPrime = 1;
  if(num == 2)
    return 1;
  for (uint64_t i = 3; i <= numsqrt && isPrime; i+=2){
    if(num%i == 0)
      isPrime = 0;
  }
  return isPrime;
}

static primeS primeState[2] = {{0, -1}, {0, -1}};

void initPrime(int screenId){
  primeState[screenId].cantPrinted = 0;
  primeState[screenId].lastPrime = -1;
}

int prime(int argc, char* argv[], int screenId){
  //sleep casero para debuggear
  for (size_t i = 0; i < 100000000; i++)
  {
     code 
  }
  
  int toPrint;

  if(primeState[screenId].lastPrime == -1)
    toPrint = 2;
  else if(primeState[screenId].lastPrime == 2)
    toPrint = 3;
  else{
    int i = primeState[screenId].lastPrime;
    do{
      i+=2;
    }
    while(!isPrime(i));
    toPrint = i;
  }
  
  primeState[screenId].lastPrime = toPrint;

  
  //_fprintf(STDOUT, "La raiz de %d es %d\n", primeState[screenId].cantPrinted, _sqrt(primeState[screenId].cantPrinted)); //para testiar el sqrt
  _fprintf(STDOUT, "El primo nro %d es %d\n", ++(primeState[screenId].cantPrinted), toPrint);
  return 0;
}

*/
int primes(){
  int num = 2;
  int aux = 2;
  _fprintf(STDOUT, "prime : %d",num);
  num++;
  while(1){
    num += 2;
    int m = num/2;
    for(int i = 2 ; i <= m; i++){    
      if(num % i == 0)  
        break;  
    }
    sys_wait(1);
    _fprintf(STDOUT, "prime : %d", num);
  }
  sys_exit();
  return num;
}

