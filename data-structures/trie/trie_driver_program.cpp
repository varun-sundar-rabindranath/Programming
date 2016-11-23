#include <iostream>
#include <vector>
#include <string> 
#include "trie.hpp"
using namespace std;

int main() {
  trie t;

  init_trie(&t);

  vector<string> vec_s;

  vec_s.push_back("wow");
  vec_s.push_back("wowerdfd");
  vec_s.push_back("wowdfd");
  vec_s.push_back("helllo");
  vec_s.push_back("hello");
  vec_s.push_back("he");
  vec_s.push_back("h");
  vec_s.push_back("h");
  vec_s.push_back("hell");
  vec_s.push_back("we");
  vec_s.push_back("war");
  vec_s.push_back("poltergeist");
  vec_s.push_back("poltergeists");
  vec_s.push_back("polstergeist");

  for(int i = 0; i < vec_s.size(); i++) {
    add_string(&t, vec_s[i]);
  }

  print_trie(&t);

  vector<pair<string, int> > word_freq;

  word_freq = get_word_frequency(&t);

  for(int i = 0; i < word_freq.size(); i++) {
    cout<<word_freq[i].first<<" --> "<<word_freq[i].second<<endl;
  }


  destroy_trie(&t);

  return 0;
}
