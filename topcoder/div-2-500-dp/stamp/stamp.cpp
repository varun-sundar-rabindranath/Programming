#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define SLEN 55

int dp[SLEN];

void fill_dp(int val) {
  for(int i = 0; i < SLEN; i++) {
    dp[i] = val;
  }
}

class Stamp {
public : 
  int getMinimumCost(string s, int lc, int pc) {
    fill_dp(-1);

    vector<vector<int> > labels;
    labels.resize(s.length());

    dp[0] = lc + pc;
    labels[0].push_back(1);


    for(int i = 1; i < s.length(); i++) {
      if(s[i] == s[i - 1]) {
      } else {
	bool one_present = false;
	/* Check if we have a label of size 1 in the lot */
	for(int j = 0; j < labels[i - 1].size(); j++) {
	  if(labels[i - 1][j] == 1) { one_present = true; break; }
	}

	if (one_present) {
	  dp[i] = dp[i - 1] + pc;
	} else {
	  dp[i] = dp[i - 1] + pc + lc;
	}
      }
    }

  }
}
