#include <iostream>
using namespace std;

class base_class {
public:
  int class_type;

  base_class(int a) {
    class_type = 2;
  }
};

class derived_class : public base_class {
public:
  derived_class():base_class(12) {
    //class_type = 1;
  }
};

int main() {
  base_class bc1 = base_class(32);
  derived_class dc1;
  cout<<"Base class "<<bc1.class_type<<endl;
  cout<<"Derived class "<<dc1.class_type<<endl;
  return 0;
}
