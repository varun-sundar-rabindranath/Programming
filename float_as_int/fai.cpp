#include <iostream>
#include <cstring>
using namespace std;

typedef union union_fi {
  float uf;
  int ui;
}union_fi;

int main() {

  float f = 1.23459823;
  int encoded_i;
  float decoded_f;

  union_fi ufi;
  ufi.uf = f;
  encoded_i = ufi.ui;
  memcpy(&decoded_f, &encoded_i, 4);

  cout<<"f "<<f<<endl;
  cout<<"ufi.fi "<<ufi.uf<<endl;
  cout<<"ufi.ui "<<ufi.ui<<endl;
  cout<<"encoded_i "<<encoded_i<<endl;
  cout<<"decoded_f "<<decoded_f<<endl;

  return 0;
}
