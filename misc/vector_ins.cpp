#include<iostream>
#include <vector>
using namespace std;

void print_vec(vector<int> &a) {
  for(int i = 0; i < a.size(); i++) {
    cout<<a[i]<<" ";
  }
  cout<<endl;
}


int main() {
  vector<int> a;
  a.push_back(10);
  
  a.insert(a.begin() + 0, 11);
  a.insert(a.end(), 11);

  print_vec(a);
  return 0;
}
