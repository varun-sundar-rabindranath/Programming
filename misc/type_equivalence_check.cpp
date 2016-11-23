#include <iostream>
using namespace std;

typedef float float_type;
typedef double double_type;

int main() {

  if(float_type == float) {
    fprintf(stderr, "float_type is float \n");
  }
  if(double_type == double) {
    fprintf(stderr, "double_type is double\n");
  }
}
