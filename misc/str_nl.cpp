#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> get_lines(string s) {

  vector<string> ret_vec;

  while(s.find_first_of('\n') != string::npos) {

  }

  return ret_vec;
}

int main() {
  string str = "hello world1\nhello world2\nhello world3\n\n";

  vector<string> lines = get_lines(str);

  return 0;
}
