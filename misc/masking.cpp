#include <iostream>
using namespace std;

#define GET_LEFT(val) ((val & 0xFFFF0000) >> 16)
#define GET_RIGHT(val) (val & 0x0000FFFF)

int main() {
  int values = 0x0FF01234;
  cout<<(GET_LEFT(values))<<endl;
  cout<<(GET_RIGHT(values))<<endl;
  //cout<<( (GET_LEFT(values)) & ( (GET_RIGHT(values) << 16) | 0x0000FFFF ) )<<endl;
  int new_left =  ( (GET_RIGHT(values) << 16) | 0x0000FFFF ); 
  int new_right = GET_LEFT(values) | 0xFFFF0000;
  cout<<"new left "<<new_left<<endl;
  cout<<"new value "<<(new_right & new_left)<<endl;
  return 0;
}
