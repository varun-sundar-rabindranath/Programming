#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <string>
using namespace std;

/* Checks if c is a low case alphabet */
bool is_low_alpha(char c) {
  if(c >= 97 && c <= 122) {
    return true;
  }
  return false;
}

/* Checks if the string is made off low case alphabets */
bool is_string_alpha(string s) {
  bool ret_val = true;
  for(int i = 0; i < s.length(); i++) {
    ret_val = ret_val && is_low_alpha(s[i]); 
  }
  return ret_val;
}


/* descending sort operation on vector<pair<string, int> >; */ 
bool sort_op(const pair<string, int> &left, const pair<string, int> &right) {
  return left.second > right.second;
}
#endif
