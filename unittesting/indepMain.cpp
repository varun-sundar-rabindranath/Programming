#include <stdio.h>
#include <iostream>

using namespace std;

void processEven(int a) {
  /* Will give 100% statement coverage if only even number is passed */
  /* Will not give 100% statement coverage if only even number is passed */
  if (a%2 == 0) {
    fprintf(stderr, "Even number \n");
  } else {
    fprintf(stderr, "Odd number \n");
  }
}

void processNumber(int a) {
  if (a%2 == 0) {
    fprintf(stderr, "Even number \n");
  } else {
    fprintf(stderr, "Odd number \n");
  }

  /* Unreachable code */
  if(0) {
    fprintf(stderr, "Unreachable code \n");
  }
}

void dummy() {
  if(1) {
    fprintf(stderr, "dummy");
  }
}

int main() {
  processEven(2);
  processNumber(1);
  processNumber(2);
  return 0;
}
