#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string>

using namespace std;

class A {
 public:
   char *a_ptr;
   A() {
     cerr<<"Inside A constructor !"<<endl;
     a_ptr =(char *)malloc(100);
   }
   ~A() {
    cerr<<"Inside A destructor !"<<endl;
     free(a_ptr);
   }
};

struct use_stuff {
 string var1;
 string var2;
 int int_obj;
};

int main() {

#if 1
  struct use_stuff *us = new use_stuff;
  //us->var1 = string("WOW");
  //us->var2 = string("WON");
  delete us;
#endif

#if 0
  /* using malloc and free */
  struct use_stuff *us = NULL;
  us = (struct use_stuff *)malloc(sizeof(struct use_stuff));
  assert(us != NULL);

  us->var1 = string("WOW");
  us->var2 = string("WON");

  free(us);
#endif
  return 0;
}
