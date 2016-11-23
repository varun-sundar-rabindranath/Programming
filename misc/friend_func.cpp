#include <iostream>
using namespace std;

class base_class {
private :
  int private_class_type;
public :
  int public_class_type;
  friend void common_func(base_class); 
};

void common_func(base_class bc) {
  cout<<"Friend - class_type "<<bc.public_class_type<<endl;
  cout<<"Friend - class_type "<<bc.private_class_type<<endl;
}

int main() {
  base_class bc;
  common_func(bc);
  return 0;
}
