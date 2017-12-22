#include <iostream>

class A {

  A(int a_, int b_) {
    a = a_;
    b = b_;
  }

  int a;
  int b;
};

int main() {

  A a1(10, 20); A a2;
}
