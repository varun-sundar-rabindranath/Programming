#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
using namespace std;

#define N 1000000
#define MAX_ELEM 1000000

int main() {
  srand(time(NULL));

  fprintf(stderr, "%d\n", N);
  /* Generate N random #s */
  for(int i = 0; i < N; i++) {
    fprintf(stderr, "%d\n", rand() % MAX_ELEM);
  }
  return 0;
}
