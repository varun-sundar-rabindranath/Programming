#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <map>
#include <set>
#include <climits>
#include "trie.hpp"
#include "utility.hpp"
using namespace std;

#define CLASS 8
#define NUM_CANDIDATE_WORDS 3
#define VALID_FREQ_PERCENT .40
int normalization_words = 0;
#define NORMALIZED_VAL(curr_freq, curr_num_words) ((normalization_words * curr_freq) / curr_num_words)

typedef struct class_info {
  int classification; 
  vector<pair<string, int> > word_frequency;
  map<string, int> string_to_frequency;
  int words_in_class;
  trie t;
} class_info;

void init_class_info(class_info *ci, int belongs_to_class) {
  ci->classification = belongs_to_class;
  ci->words_in_class = 0;
  ci->word_frequency.clear();
  init_trie(&(ci->t));
}

void add_to_trie(string line, vector<class_info> &c) {
  char classified = line[0];
  int classified_into = atoi(&classified) - 1;

  string str_to_add = "";
  for(int i = 2; i < line.size(); i++) {
    if(line[i] == ' ') {
      assert(is_string_alpha(str_to_add));
      add_string(&(c[classified_into].t), str_to_add);
      c[classified_into].words_in_class++;
      str_to_add = "";
    } else {
      str_to_add += line[i];
    }
  }
  if(str_to_add != "") {
    assert(is_string_alpha(str_to_add));
    add_string(&(c[classified_into].t), str_to_add);
    c[classified_into].words_in_class++;
  }
}

void fill_class_info(string fname, vector<class_info> &c) {
  fstream fs;
  fs.open(fname.c_str(), fstream::in);

  string line;

  string first_line;
  getline(fs, first_line);
  int num_lines = atoi(first_line.c_str());

  cerr<<"Num lines - "<<num_lines<<endl; 

  while(getline(fs, line)) {
    add_to_trie(line, c);
  }

  fs.close();
}

void get_frequency(vector<class_info> &c) {
  for(int i = 0; i < CLASS; i++) {
    c[i].word_frequency = get_word_frequency(&(c[i].t));
  }
  /* Assert that frequency of all words sums up to num_words */
  for(int i = 0; i < CLASS; i++) {
    int num_words = c[i].words_in_class;
    int w = 0;
    for(int j = 0; j < c[i].word_frequency.size(); j++) {
      w += c[i].word_frequency[j].second;
    }
    assert(w == num_words);
  }
}

void map_string_to_frequency(vector<class_info> &c) {
  for(int i = 0; i < CLASS; i++) {
    assert(c[i].word_frequency.size() != 0);
    for(int j = 0; j < c[i].word_frequency.size(); j++) {
      string s = c[i].word_frequency[j].first;
      int f = c[i].word_frequency[j].second;
      c[i].string_to_frequency.insert(pair<string, int> (s, f));
    }
  }
}

int num_words_of_class_with_least_words(vector<class_info> &c) {
  int min_words_in_doc = INT_MAX;
  for(int i = 0; i < CLASS; i++) {
    min_words_in_doc = min(min_words_in_doc, c[i].words_in_class);
  }

  assert(min_words_in_doc != -1);
  return min_words_in_doc;
}

void sort_words_based_on_freq(vector<class_info> &c) {
  for(int i = 0; i < CLASS; i++) {
    sort(c[i].word_frequency.begin(), c[i].word_frequency.end(), sort_op);
  }

#if 0
  for(int i = 0; i < CLASS; i++) {
    cout<<"======== CLASS "<<i + 1<<" ============"<<endl;
    for(int j = 0; j < c[i].word_frequency.size(); j++) {
      cout<<c[i].word_frequency[j].first<<" --> "<<c[i].word_frequency[j].second<<" --> "<<NORMALIZED_VAL(c[i].word_frequency[j].second, c[i].words_in_class)<<endl;
    }
  }
#endif
}

set<string> get_feature_words(vector<class_info> &c) {
  set<string> feature_words;

  set<string>::iterator set_it;

  for(int i = 0; i < CLASS; i++) {
    int num_words_selected = 0;
    int num_words_in_class = c[i].words_in_class;
    for(int j = 0; j < c[i].word_frequency.size() && num_words_selected < NUM_CANDIDATE_WORDS; j++) {
      string candidate = c[i].word_frequency[j].first;

      /* if word already selected; then continue */
      if(feature_words.find(candidate) != feature_words.end()) {
	continue;
      }

      float candidate_freq = (float)(c[i].string_to_frequency.find(candidate))->second;

      candidate_freq = NORMALIZED_VAL((int)candidate_freq, num_words_in_class);

      bool appears_in_all = true;

      vector<float> all_class_freq;
      /* This word should never appear in all of the eight doc class */
      for(int k = 0; k < CLASS; k++) {
	if(k == i) { /* Same class */ continue; }
	map<string, int>::iterator it = c[k].string_to_frequency.find(candidate);
	appears_in_all = appears_in_all && (it != c[k].string_to_frequency.end());
	all_class_freq.push_back(NORMALIZED_VAL(it->second, c[k].words_in_class));
      }

      float avg_freq = 0;
      for(int freq_iter = 0; freq_iter < all_class_freq.size(); freq_iter++) {
	avg_freq += all_class_freq[freq_iter];
      }
      avg_freq /= all_class_freq.size();

      if(appears_in_all && (avg_freq / candidate_freq) > VALID_FREQ_PERCENT) {
	/* We dont need this word; select a diff feature */ 
	continue;
      } else {
	num_words_selected++;
	feature_words.insert(candidate);
      }
    }
  }

  return feature_words;
}

void generate_feature_vectors(set<string> &feature_words, vector<class_info> &c) {
  set<string>::iterator set_it;

  /* Print all feature words */
  for(set_it = feature_words.begin(); set_it != feature_words.end(); set_it++) {
    cout<<*set_it<<" ";
  }
  cout<<endl;

  /* Print the normalized frequencies */
  for(int i = 0; i < CLASS; i++) {
    for(set_it = feature_words.begin(); set_it != feature_words.end(); set_it++) {
      int normalized_freq = 0;
      if(c[i].string_to_frequency.find(*set_it) != c[i].string_to_frequency.end()) {
	normalized_freq = NORMALIZED_VAL(c[i].string_to_frequency.find(*set_it)->second, c[i].words_in_class);
      }
      cout<<normalized_freq<<" ";

      //cout<<NORMALIZED_VAL((c[i].string_to_frequency.find(*set_it))->second, c[i].words_in_class)<<" ";
      //cout<<(c[i].string_to_frequency.find(*set_it))->second<<" ";
    }
    cout<<endl;
  }
}

void generate_features(set<string> feature_words, string input_file, vector<int> &y, vector<vector<int> > &feature_vec) {
  fstream fs;
  fs.open(input_file.c_str(), fstream::in);

  string line;

  string first_line;
  getline(fs, first_line);
  int num_lines = atoi(first_line.c_str());

  set<string>::iterator it;

  while(getline(fs, line)) {

    trie t;
    init_trie(&t);

    vector<int> count;

    string str = "";
    for(int i = 2; i < line.size(); i++) {
      if(line[i] == ' ') {
        assert(is_string_alpha(str));
	it = feature_words.find(str);
	add_string(&t, str);
        str = "";
      } else {
        str += line[i];
      }
    }
    if(str != "") {
      assert(is_string_alpha(str));
      add_string(&t, str);
    }

    set<string>::iterator set_it;
    for(set_it = feature_words.begin(); set_it != feature_words.end(); set_it++) {
      count.push_back(get_str_count(&t, *set_it));
    }

    feature_vec.push_back(count);

    char dc[2]; dc[0] = line[0]; dc[1] = '\0';
    assert(atoi(dc) > 0 && atoi(dc) <= 8);
    y.push_back(atoi(dc));

    destroy_trie(&t);
  }

  fs.close();

}

void print_words_in_class(vector<class_info> &c) {
  for(int i = 0; i < CLASS; i++) {
    cout<<"Class "<<c[i].classification<<" : "<<c[i].words_in_class<<endl;
  }
}

vector<class_info> c(CLASS);

int main(int argc, char *argv[]) {
  
  if(argc != 2) {
    cerr<<"Insufficent command line args ... Exiting \n";
    return -1;
  }

  string input_file = string(argv[1]);

  cout<<" Input file : "<<input_file<<endl;

  for(int i = 0; i < CLASS; i++) {
    init_class_info(&c[i], i + 1);
  }

  /* Convert the data from input_file into tries */
  fill_class_info(input_file, c);

  /* Get frequency of all words in all classes */
  get_frequency(c);

  /* Map strings in a class to their freq */
  map_string_to_frequency(c);

  normalization_words = num_words_of_class_with_least_words(c);

  /* Sort words in the descending order of frequency */
  sort_words_based_on_freq(c);

  set<string> feature_words;
  /* get feature words */
  feature_words = get_feature_words(c);

  /* Print num words n each class */
  //print_words_in_class(c);

  //generate_feature_vectors(feature_words, c);

  vector<vector<int> > x; vector<int> y;

  generate_features(feature_words, input_file, y, x);

  /* Print feature words */
  cout<<"========= Feature words ========== "<<endl;
  set<string>::iterator set_it;
  cout<<feature_words.size()<<endl;
  for(set_it = feature_words.begin(); set_it != feature_words.end(); set_it++){
    cout<<*set_it<<" ";
  }
  cout<<endl;

  cout<<x.size()<<endl;
  cout<<x[0].size()<<endl;

  /* Print features */
  for(int i = 0; i < x.size(); i++) {
    /* Print feature */
    for(int j = 0; j < x[i].size(); j++) {
      fprintf(stdout, "%d ", x[i][j]);
    }
    cout<<endl;
  }

  cout<<y.size()<<endl;
  for(int i = 0; i < y.size(); i++) {
    fprintf(stdout, "%d \n", y[i]);
  }

  for(int i = 0; i < CLASS; i++) {
    destroy_trie(&(c[i].t));
  }

  return 0;
}
