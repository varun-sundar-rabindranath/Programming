#include <iostream>
#include <stdio.h>

using namespace std;

template<int N>
struct tp_template {
  typedef double Type;
};

template<> 
struct tp_template<1> {
  typedef char Type;
};

template<>
struct tp_template<2> {
  typedef float Type;
};

int main() {

  int c;
  cin>>c;

  fprintf(stderr, " N = 1 - size %lu \n ", sizeof(tp_template<1>::Type));
  fprintf(stderr, " N = 2 - size %lu  \n ", sizeof(tp_template<2>::Type));
  fprintf(stderr, " N = 3 - size %lu  \n ", sizeof(tp_template<c>::Type));

  return 0;
}
