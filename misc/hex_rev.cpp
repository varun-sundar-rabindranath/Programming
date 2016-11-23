#include <iostream>
#include <vector>
#include <cassert>
#include <string>

#define PATTERN "4a46494600"

using namespace std;

vector<string> tokenizer(string str) {
  vector<string> ret;
  string s = "";
  for(int i = 0; i < str.length(); i++) {
    if(str[i] == ' ') {
      if(s != "")
	ret.push_back(s);
      s = "";
    } else {
      s += str[i];
    }
  }
  if(s != "") 
    ret.push_back(s);
  return ret;
}

string str_reverse(string s) {
  for(int i = 0; i < s.length() / 2; i++) {
    char temp = s[i];
    s[i] = s[s.length() - 1 - i];
    s[s.length() - 1 - i] = s[i]; 
  }
  return s;
}

bool search_string(string pattern, string text) {
  for(int i = 0; i < text.length(); i++) {
    if(text.length() - i >= pattern.length()) {
      bool same_string = true;
      for(int j = 0; j < pattern.length(); j++) {
	if(text[i + j] != pattern[j]) {
	  same_string = false;
	  break;
	}
      }
      if(same_string) { return true; }
    }
  }
  return false;
}

int main() {
  string s;
  getline(cin, s);

  while(s != "") {

    string text = "";

    vector<string> s_vec = tokenizer(s);
    if(s_vec.size() > 0)
      text += s_vec[0] + " ";
      
    for(int j = 1; j < s_vec.size(); j++) {
       assert(s_vec[j].length() == 4);
       string front = s_vec[j].substr(0, 2);
       string back = s_vec[j].substr(2, 2); 
       string result = back + front;
       text += result;
    }

    if(search_string(PATTERN, text)) { cout<<text<<endl; } 

    getline(cin, s);
  }
  return 0;
}

