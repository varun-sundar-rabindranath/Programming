#include <iostream>
using namespace std;

int main() {
  int n = 9;
  int p = 13;

  for (int i = 1; i < p; i++) {
    cout<<i * n <<" - "<<(i * n) % p<<endl; 
  }
  return 0;
}
