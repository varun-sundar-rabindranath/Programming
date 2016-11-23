#include <iostream>
using namespace std;

class B;
class A {
  public :
          friend void friend_ab();
          A() {
            a_intval = 34;
          }
  private : 
          int a_intval;
};

class B {
  public :
          friend void friend_ab();
          B() {
            b_intval = 12;
          }
  private : 
          int b_intval;
};

void from_friend(A a, B b) {
  cout<<"A - a_intval - "<<a.a_intval<<endl;
  cout<<"B - b_intval - "<<b.b_intval<<endl;
}

void friend_ab() {
  A a;
  B b;
  from_friend(a, b);
}

int main() {
  friend_ab();
  return 0;
}

