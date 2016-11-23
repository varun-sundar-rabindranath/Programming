#include <iostream>
#include <cassert>
#include <stdio.h>
using namespace std;

int main() {
  FILE *fp = fopen("./jpeg-home.jpg", "a");
  char char_arr[2000] = "taan taan da taan !! tan tan !! kickbuttowski !!";
  int num_write_bytes = fwrite(char_arr, sizeof(char), 2000, fp);
  assert(num_write_bytes == 2000);
  fclose(fp);
  return 0;
}
