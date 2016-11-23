#include <iostream>
#include <vector>
#include <stack>
using namespace std;

#define MAX_N 55

int dp[55];

void fill_dp(int val) {
  for(int i = 0; i < MAX_N; i++) {
    dp[i] = val;
  }
}

int calc_score(vector<pair<int, int> > &combo, vector<int> &scores) {
  int ret_val = 0;
  for(int i = 0; i < combo.size(); i++) {
    ret_val += scores[ combo[i].first - 1 ] * combo[i].second;
  }
  return ret_val;
}

vector<int> best_combo(vector<int> &best_j) {
  int j_index = best_j.size() - 1;

  vector<int> ret_vec(MAX_N);
  for(int i = 0; i < MAX_N; i++) {
    ret_vec[i] = 0;
  }

  stack<int> to_look_at;
  to_look_at.push(best_j[best_j.size() - 1]);

  while(!to_look_at.empty()) {
    int curr_j = to_look_at.top();
    to_look_at.pop();



  }
}

class P8XGraphBuilder {
public :
  int solve(vector<int> scores) {

    

    fill_dp(-1);

    dp[0] = 0;
    dp[1] = scores[0]; 

    int init_score = 0;
    init_score = scores[ scores.size() - 1 ] + scores.size() * scores[0];

    int max_score = -1;

    vector<int> best_j;
    best_j.push_back(0);
    best_j.push_back(1);

    for(int i = scores.size() - 1; i > 0; i--) {
      int to_split = scores.size() - i;
      int max_to_split = -1;
      int successful_j = -1;

      for(int j = 1; j <= to_split; j++) {
	int num_js = to_split / j;
	int remainder = to_split % j;
	int this_score = num_js * scores[j - 1] + scores[remainder - 1];

	if(this_score > max_to_split) {
	  max_to_split = this_score;
	  successful_j = j;
	}
      }

      dp[to_split] = max_to_split;
      best_j.push_back(successful_j);
    }



    return max_score;
  }
};
