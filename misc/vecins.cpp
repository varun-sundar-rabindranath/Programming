#include <iostream>
#include <vector>
using namespace std;

int main() {
  vector<int> a;
  a.push_back(4);
  a.push_back(7);
  a.push_back(10);
  a.push_back(12);
  a.insert(a.begin() + 3, 11);
  for(int i = 0; i < a.size(); i++) {
    cout<<a[i]<<" ";
  }
  return 0;
}
