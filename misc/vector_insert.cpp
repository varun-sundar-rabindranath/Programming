#include <iostream>
#include <vector>
using namespace std;

int main() {
  vector<int> a;
  a.push_back(1);
  a.push_back(2);

  vector<int>::iterator it = a.begin();

    if(it == a.begin()) {
      a.insert(it, 1000);
    }

    for(it = a.begin(); it != a.end(); it++) {
      if(*it == 2) {
	break;
      }
    }

    a.insert(it, 900);

  it = a.end();
  a.insert(it, 200);

  for(int i = 0; i < a.size(); i++) {
    cout<<a[i]<<" ";
  }

  return 0;

}
