#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;

void do_fork() {
  const int i = 5;
  fork();
  fprintf(stderr, " %d ", ++i);
}

int main() {
  do_fork();
  do_fork();
  return 0;
}
