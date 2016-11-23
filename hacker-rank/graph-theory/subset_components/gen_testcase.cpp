#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

#define ELEMENTS 20

int main() {

  srand(time(NULL));

  cout<<ELEMENTS<<endl;
  for(int i = 0; i < ELEMENTS; i++) {
    unsigned long long int rand_val = (unsigned long long int)rand();
    rand_val = rand_val << 32;
    rand_val = rand_val | rand();
    cout<<rand_val<<" ";
  }

  return 0;
}
