#include<iostream>
#include<cassert>
#include <cstdlib>
using namespace std;

int main() {
  int *intarr = NULL;
  intarr = (int *)calloc(10, sizeof(int));
  assert(intarr);

  cout<<"Size of intarr - "<<sizeof(intarr)<<endl;
  free(intarr);

  return 0;
}
