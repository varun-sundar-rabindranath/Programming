#include <iostream>
using namespace std;

typedef enum Mode{
  MODE_START = -1,
  MODE_ZERO,
  MODE_ONE,
  MODE_TWO,
  MODE_NUM,
  MODE_END,
}Mode;

int main() {
  cout<<"MODE_START - "<<MODE_START<<endl;
  cout<<"MODE_ZERO  - "<<MODE_ZERO<<endl;
  cout<<"MODE_ONE   - "<<MODE_ONE<<endl;
  cout<<"MODE_TWO   - "<<MODE_TWO<<endl;
  cout<<"MODE_NUM   - "<<MODE_NUM<<endl;

  return 0;
}
