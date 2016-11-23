#include <iostream>
using namespace std;

class base_class;
class derived_class;

class base_class {
  public : 
    int base_class_var;
    int class_type;

    void common_func();

    base_class() {
      class_type = 0;
    }
};

class derived_class : public base_class {
  public :
    int derived_class_var;
    void common_func() {
      cout<<"Derived - class type - "<<class_type<<endl;
    }

    derived_class() {
      class_type = 1;
    }
}; 
    void base_class::common_func() {
      if(class_type == 0) {
	cout<<"Base - class type - "<<class_type<<endl;
      } else if(class_type == 1) {
	void * tc = (void *)this;
	derived_class *dc = (derived_class *)tc;
	dc->common_func();
      }
    }


int main() {
  base_class bc;
  derived_class dc;
  bc.common_func();
  dc.common_func();

  base_class dummy = dc;
  dummy.common_func();

  void *tc_dc = &bc;
  derived_class *varun = (derived_class *)tc_dc;
  varun->common_func();  
  return 0;
}
