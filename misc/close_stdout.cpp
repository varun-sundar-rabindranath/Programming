#include <iostream>
#include <fstream>
using namespace std;

int main() {
  fprintf(stdout, "Before stdout close \n");
  fclose(stdout);
  while(1)
    fprintf(stdout, "After stdout close \n");
  return 0;
}
