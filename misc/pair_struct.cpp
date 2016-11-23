#include <iostream>
#include <vector>
using namespace std;

typedef struct stype {
  int a;
  stype() {
    a = 10;
  }

  stype(int v) {
    a = v;
  }
}stype;

int main() {
  vector<pair<int, stype> > s_vec;

  s_vec.push_back(pair<int, stype>(4, stype(4)));
  s_vec.push_back(pair<int, stype>(5, stype(5)));
  s_vec.push_back(pair<int, stype>(6, stype(6)));
  s_vec.push_back(pair<int, stype>(7, stype(7)));
  s_vec.push_back(pair<int, stype>(8, stype(8)));

  for(int i = 0; i < s_vec.size(); i++) {
    cout<<s_vec[i].second.a<<endl;
  }
  return 0;

}
