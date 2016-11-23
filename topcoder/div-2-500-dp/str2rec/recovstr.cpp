#include <iostream>
#include <vector> 
#include <string>
#include <cassert>

using namespace std;

bool res_found = false;
string res;

class StrIIRec {
public : 
string recovstr(int n, int minInv, string minStr) {

  string s = "";
  char c = 'a';
  for(int i = 0; i < n; i++) {
    s += " ";
    s[i] = c + i;
  }

  vector<bool> alpha(26);
  for(int i = 0; i < 26; i++) {
    alpha[i] = false;
  }

  for(int i = 0; i < minStr.size(); i++) {
    alpha[minStr[i] - 97] = true;
  }

  for(int i = 0; i < n; i++) {
    if(alpha[i] == false) {
      char c[2]; c[0] = i + 97;
      c[1] = '\0';
      minStr += string(&c[0]);
    }
  }

  rec(s, 0, minInv, minStr); 

  return res;
}

private : 
int get_inversion(string &s) {
  int inv_count = 0;

  for(int i = 0; i < s.length(); i++) {
    for(int j = i + 1; j < s.length(); j++) {
      if(s[i] > s[j]) { inv_count++; }
    }
  }

  return inv_count;
}

string swap(string s, int i, int j) {
  assert(i >= 0);
  assert(j < s.length());

  char c = s[i];
  s[i] = s[j];
  s[j] = c;

  return s;
}

void rec(string s, int idx, int minInv, string minStr) {

  for(int i = idx; i < s.length(); i++){
     string to_rec = swap(s, idx, i);
     if(to_rec.compare(0, idx + 1, minStr) >= 0) 
     { 
        if(get_inversion(to_rec) >= minInv) { res_found = true; res = to_rec;}
	
	if(!res_found) {
	  rec(to_rec, idx + 1, minInv, minStr); 
	}
     }
  }
}

};

