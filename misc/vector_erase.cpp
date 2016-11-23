#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

void rem(vector<int> &vec, int v) {
  vector<int>::iterator it;
  it = vec.begin();
   
   while(it != vec.end()) {

    if(*it == 0 || *it == 9 || *it == 5 || *it == 6 || *it == 2 || *it == 8) {
      bool is_nine = (*it == 9);

      cout<<"before "<<*it <<"  ";
      it = vec.erase(it);
      cout<<"after "<<*it<<endl;

      if(is_nine) {
        cout<<"is end "<<(it == vec.end())<<endl;
      }

    } else {
      it++;
    }
   } 
}

void print(vector<int> &vec) {
  for(unsigned int iter = 0; iter < vec.size(); iter++) {
    cout<<vec[iter]<<" ";
  }
  cout<<endl;
}

int main() {
  vector<int> v;

  for(int val = 0; val < 10; val++) {
    v.push_back(val);
  }


  print(v);

  rem(v, 0);
  print(v);

//  rem(v, 9);
//  print(v);
//
//  rem(v, 5);
//  print(v);
//
//  rem(v, 6);
//  print(v);
//
//  rem(v, 1);
//  print(v);
//
//  rem(v, 8);
//  print(v);
//
//  rem(v, 7);
//  print(v);

  return 0;
}
