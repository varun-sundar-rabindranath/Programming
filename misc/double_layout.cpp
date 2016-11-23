#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
using namespace std;

#define SIGN_MASK 0x800000000000000
#define DECIMAL_MASK 0x7FF0000000000000
#define FRACTION_MASK 0x000FFFFFFFFFFFFF

#define GET_SIGN(a) ( (a & SIGN_MASK) >> 63)
#define GET_DECIMAL(a) ( (a & DECIMAL_MASK) >> 52)
#define GET_FRACTION(a) ( (a & FRACTION_MASK) )

union int_double {
    unsigned long long int int_val;
    double double_val;
};
void get_bits(int_double id) {
  vector<int> bits;
  for(int i = 0; i < 64; i++) {
    unsigned long long int v = 1;
    bits.push_back(( id.int_val & (v << i)) >> i);
  }
  cout<<id.double_val<<" - ";
  assert(bits.size() == 64);
  for(int i = bits.size() - 1; i >= 0; i--) {
    cout<<bits[i];
  }
  cout<<endl;
}

int main() {
  double a = -3.0;
  unsigned long long int val = a;

  int_double id;
  id.double_val = a;

  for(int i = -10; i < 10; i++) {
    id.double_val = (double)i;
    get_bits(id);
  }


//  cout<<"double a "<<id.double_val<<endl;
//  cout<<"ull a "<<id.int_val<<endl;
//  cout<<"Double sign "<<GET_SIGN(id.int_val)<<endl;
//  cout<<"Double decimal "<<GET_DECIMAL(id.int_val)<<endl;
//  cout<<"Double fraction "<<GET_FRACTION(id.int_val)<<endl;


  return 0;
}
