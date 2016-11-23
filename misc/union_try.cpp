#include <iostream>
#include <cstdio>
using namespace std;

typedef union char_to_int {
  int int_val;
  char char_arr[4];
} char_to_int;

int main() {
  char_to_int ins;
  ins.char_arr[0] = 1;
  ins.char_arr[1] = 1;
  ins.char_arr[2] = 0;
  ins.char_arr[3] = 0;

  fprintf(stderr, "intval - %d \n", ins.int_val);

  int val = 0xa;
  cout<<val<<endl;

  return 0;
}
