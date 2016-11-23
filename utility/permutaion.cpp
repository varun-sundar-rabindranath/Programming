#include <iostream>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

string swap(string s, int i, int j) {
  assert(i >= 0);
  assert(j < s.length());

  char c = s[i];
  s[i] = s[j];
  s[j] = c;

  return s;
}

void permute(string s, int idx) {
  for(int i = idx; i < s.length(); i++){
    permute(swap(s, idx, i), idx + 1);
  }

  if(idx == s.length()) {
    cout<<s<<endl;
  }
}


int main() {
  permute("varun", 0);
  return 0;
}
