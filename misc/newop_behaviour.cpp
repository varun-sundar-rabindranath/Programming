#include <iostream>
using namespace std;

int *myfunc() {
  int *a = new int();
  return a;
}

int main() {

  int *b = myfunc();
  delete b;
  return 0;
}
