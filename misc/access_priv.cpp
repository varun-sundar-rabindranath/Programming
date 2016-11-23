#include <iostream>
#include <cstdio>

class A;
class B {
  public :
    void do_all(int set_it_pls) {
      set_neighbour_a(set_it_pls);
    }

    B(A *a_ptr_) {
      a_ptr = a_ptr_;
    }
  private :
    int b;
    A *a_ptr;
    void set_neighbour_a(int set_it_pls);
};

class A {
  
  public :
    A() {
      a = 12;
    }

    int get_a() {
      return a;
    }

  friend class B;

  private :
    int a;
};

void B::set_neighbour_a(int set_it_pls) {
  a_ptr->a = set_it_pls;
}

int main() {
  A a_obj;
  B b(&a_obj);

  printf("Before set %d \n", a_obj.get_a());

  b.do_all(232);

  printf("%d \n", a_obj.get_a());

  return 0;
}


