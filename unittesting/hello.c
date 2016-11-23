#include <stdio.h>
#include "hello.h"

void printHello(int flag) {
  if(flag) {
    fprintf(stderr, "Hello \n");
  }
}
