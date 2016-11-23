#include <iostream>
#include <stdio.h>
using namespace std;

union double_int {
  double a;
  long long int b;
};

int main() {
  double var = 12345.6789;
  double_int di;
  di.a = var;
  //di.b = (2173712813 << 32) | 1714550945;
  printf(" %lld \n", di.b);
  return 0;
}
