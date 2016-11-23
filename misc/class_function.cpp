#include <iostream>
#include <cstdio>
using namespace std;

class c {
  public: 
  int x;
  static void print_var() {
     fprintf(stderr, " wow \n");
  }
};

int main() {
  c::print_var();
  return 0;
}
