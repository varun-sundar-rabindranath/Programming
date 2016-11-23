#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <climits>

using namespace std;

#define MAX_LEN 55

int dp[MAX_LEN];

void fill_dp(int val) {
  for(int i = 0; i < MAX_LEN; i++) {
    dp[i] = val;
  }
}

class SentenceDecomposition {
public : 
  int decompose(string sentence, vector<string> valid_words) {
    fill_dp(-1);

    for(int i = 0; i < sentence.size(); i++) {
      vector<int> pos;
      pos = find_valid_positions(i, dp);

      int min_cost = INT_MAX;
      int min_pos = -2;
      for(int j = 0; j < pos.size(); j++) {
	int to_add_cost = (pos[j] == -1) ? 0 : dp[pos[j]];
	string ss = sentence.substr(pos[j] + 1, (i - (pos[j] + 1)) + 1);
	int cost = t_cost(ss, valid_words);

	if(cost == -1) { continue; }

	cost += to_add_cost;
	if(cost < min_cost) {
	  min_cost = cost;
	  min_pos = pos[j];
	}

      }

      if(min_cost == INT_MAX) {
	dp[i] = -1;
      } else {
	dp[i] = min_cost;
      }
    }
    return dp[sentence.size() - 1];
  }
private : 

  vector<int> find_valid_positions(int p, int dp[MAX_LEN]) {
    vector<int> pos;
    for(int i = 0; i < p; i++) {
      if(dp[i] != -1) {
	pos.push_back(i);
      }
    }
    pos.push_back(-1);
    return pos;
  }
  
  int compute_tcost(string s1, string s2)  {
    assert(s1.length() == s2.length());
    int cost = 0;
    for(int i = 0; i < s1.length(); i++) {
      if(s1[i] != s2[i]) {
	cost++;
      }
    } 
    return cost;
  }

  int t_cost(string s, vector<string> valid_words) {

    vector<int> t_costs;

    for(int i = 0; i < valid_words.size(); i++) {
      if(s.length() != valid_words[i].length()) {
	continue;
      }

      vector<bool> mark;
      for(int j = 0; j < s.length(); j++) {
	mark.push_back(false);
      }

      bool strings_match = true;
      string to_cmp = valid_words[i];
      for(int j = 0; j < s.length(); j++) {
	bool marked = false;
	for(int k = 0; k < to_cmp.size(); k++) {
	  if(mark[k] == false && to_cmp[k] == s[j]) {
	    mark[k] = true;
	    marked = true;
	    break;
	  }
	}

	if(!marked) {
	  strings_match = false;
	  break;
	}
      }

      if(strings_match) {
	t_costs.push_back(compute_tcost(s, valid_words[i]));
      }
    }

    if(t_costs.size() == 0) {
      /* No match */
      return -1;
    } else {
      int minval = INT_MAX;
      for(int i = 0 ; i < t_costs.size(); i++) {
	minval = min(minval, t_costs[i]);
      }
      return minval;
    }

  }
};
