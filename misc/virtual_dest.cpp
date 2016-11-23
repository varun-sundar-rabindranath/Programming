#include<iostream>
#include<typeinfo>
using namespace std;

class base_class {
public : 
  int base_var;
  base_class() {
    cerr<<"Base class constructor \n";
  }
#if 0
  virtual ~base_class() {
    cerr<<"Base class destructor \n";
  }
#else
  ~base_class() {
    cerr<<"Base class destructor \n";
  }
#endif
};

class derived_class : public base_class {
public : 
  int derived_var;
  derived_class() {
    cerr<<"Derived class constructor \n";
  }
  ~derived_class() {
    cerr<<"Derived class destructor \n";
  }
};

#define PRINT_TYPE_ID(ptr)				    \
  if(typeid(ptr).name() == typeid(base_class).name()){		    \
    cerr<<"Type base class"<<endl;			    \
  } else if(typeid(ptr).name() == typeid(derived_class).name()) {	    \
    cerr<<"Type derived class"<<endl;			    \
  } else {						    \
    cerr<<"Type no class"<<endl;			    \
  }

void print_type_id(void *ptr) {
}

void func(base_class &bc) {
  PRINT_TYPE_ID(bc);
}

int main() {
  base_class *bc1;
  derived_class *dc1 = new derived_class;
  bc1 = dc1;

  func(*bc1);

  PRINT_TYPE_ID(*bc1);
  return 0;
}
