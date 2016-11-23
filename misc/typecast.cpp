#include <iostream>
using namespace std;

int main() {
  unsigned short a = 0xFF00;

  unsigned char b = (unsigned char) a;

  cout<<"A - "<<a<<" | B - "<<(int)b<<endl;
  return 0;

}
