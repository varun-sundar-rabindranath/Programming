#include <stdio.h>
#include <stdlib.h>

int odd_or_even(int i) {
  return i%2;
}

int main() {
  char c = 10;
  char *a =(char *) malloc(10);
  a[0] = 'a';
  a[1] = 'b';
  a[2] = 'f';
  a[3] = 'c';
  a[4] = 'd';
  a[5] = 'e';
  a[6] = 'w';
  a[7] = 'w';
  a[8] = 'r';
  a[9] = '\0';

  printf( "c addr  - %p \n", &c);
  printf(" a addr - %p \n", &a);

  printf(" c - %d \n", c);
  printf(" a - %p \n", a);

  
  for(int i = 0; i < 10; i++) {
    printf("addr a[%d] -  %p \n",i, &a[i]);
  }

 // free(a);

  char *b = (char *) malloc(10);
  printf(" b - %p \n ", b);

  for(int i=0; i < 1000000; i++) {
    b = (char *) malloc(100000);
  }

  return 1;
}
