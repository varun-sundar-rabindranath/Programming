#include <iostream>
#include <cassert>
#include <cstdlib>
using namespace std;

int main() {
  int *a = NULL;
  a = (int *) malloc(100 * sizeof(int));
  assert(a != NULL);

  cout<<"A address "<<a<<endl;
  cout<<"A + 10 address"<<a + 10<<endl;

  free(a);
  return 0;
}
