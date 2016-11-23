#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
  map<string, int> stoi;

  stoi.insert(pair<string, int>("wowo", 10));
  stoi.insert(pair<string, int>("a", 10));
  stoi.insert(pair<string, int>("b", 1));
  stoi.insert(pair<string, int>("c", 9));
  stoi.insert(pair<string, int>("d", 5));

  map<string, int>::iterator it;

  for(it = stoi.begin(); it != stoi.end(); it++) {
    cout<<it->first<<" "<<it->second<<endl;
  }

  return 0;

}
