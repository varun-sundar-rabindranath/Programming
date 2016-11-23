#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;


int main() {

  char a[10];
  sprintf(a, "%d", 0 + 1);
  string str = string(a) + " 10";
  cout<<str<<endl;
  return 0;
}
