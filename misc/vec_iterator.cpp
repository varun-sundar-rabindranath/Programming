#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {

  std::vector<int> a;

  a.push_back(0);
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);
  a.push_back(5);
  a.push_back(6);
  a.push_back(7);
  a.push_back(8);
  a.push_back(9);
  a.push_back(10);

  std::vector<int>::iterator a_begin = a.begin();
  std::vector<int>::iterator a_end   = a.end();

  std::vector<int>::iterator a_iter = a.begin();

  int i = -10;

  while (i < 30) {

    cout<<i<<" "<<((a_iter + i) < a.end())<<endl;

    i++;
  }

  return 0;
}
