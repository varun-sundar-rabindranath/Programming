#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <climits>
using namespace std;

#define MAX_SONGS 55
#define MAX_SUM 55000

int dp[MAX_SONGS];
int dp2[MAX_SONGS][2][MAX_SUM];
/* 0 is no touch; 1 is chosen */
int choice[MAX_SONGS];

void fill_dp(int val) {
  for(int i = 0; i < MAX_SONGS; i++) {
    dp[i] = val;
  }
}

void fill_dp2(int val) {
  for(int i = 0; i < MAX_SONGS; i++) {
    for(int k = 0; k < 2; k++) {
      for(int j = 0; j < MAX_SUM; j++) {
        dp2[i][k][j] = val;
      }
    }
  }
}

void fill_choice(int val) {
  for(int i = 0; i < MAX_SONGS; i++) {
    choice[i] = 0;
  }
}


class ChangingSounds {
public :
  int maxFinal(vector <int> changeIntervals, int beginLevel, int maxLevel) {
    fill_dp(-1);

    vector<int> increased;
    vector<int> decreased;

    int sum = beginLevel;
    for(int i = 0; i < changeIntervals.size(); i++) {
      sum += changeIntervals[i];
      increased.push_back(changeIntervals[i]);
    }

    if(sum < maxLevel) { return sum; } 

    int ret_val =  (get_combo(increased, decreased, maxLevel - sum));
    if(ret_val == INT_MAX) {
      return -1;
    } else {
      int val = maxLevel + ret_val;
      if(val < 0) { return -1; } else { return val; }
    }
  }

private :
  int get_combo(vector<int> &vals, vector<int> &vals2, int sum) {
    fill_dp2(-1);
    //fill_choice(0);

    int result = rec(vals, 0, sum);

    return result;

    if(result == INT_MAX) {
      return result;
    }

    /* Traverse the rec tree to get the choice */
    vector<bool> choice = get_choice(vals, result);

    for(int i = 0; i < choice.size(); i++) {
      if(choice[i]) {
	vals2.push_back(vals[i]);
      } 	
    }

    vector<int>::iterator it = vals.begin();

    int iter = 0;
    while(it != vals.end()) {
      if(choice[iter]) {
	it = vals.erase(it);
      } else {
	it++;
      }
      iter++;
    }

    return result;

  }

  vector<bool> get_choice(vector<int> &vals, int result) {

    vector<bool> choice;
    for(int i = 0; i < vals.size(); i++) {
      choice.push_back(false);
    }

    int selected_sum = result;
    for(int i = 0; i < vals.size(); i++) {

      int sum1 = selected_sum - vals[i];
      int sum2 = selected_sum;

      if(sum1 == result) {
	choice[i] = true;
	break;
      }

      bool select_s1 = false;
      bool select_s2 = false;

      if(sum1 >= 0) {
	if(dp2[i][1][sum1] == result) {
	  select_s1 = true;
	}
      }

      if(dp2[i][1][sum2] == result) {
	select_s2 = true;
      }

      assert(select_s2 == true || select_s1 == true);

      if(select_s1) {
	choice[i] = true;
	selected_sum = sum1;
      } else {
	selected_sum = sum2;
      }
    }

    return choice;
 }

  int rec(vector<int> &vals, int idx, int sum) {
    if(idx >= vals.size()) {
      return sum;
    }

    if(sum == 0) {
      return 0;
    }

    int val = -1;
    int v1, v2;

    if(sum - vals[idx] < 0) {
      v1 = sum;
    } else {
      if(dp2[idx + 1][1][sum - vals[idx]] == -1) {
	dp2[idx + 1][1][sum - vals[idx]] = rec(vals, idx + 1, sum - vals[idx]);
      }
      v1 = dp2[idx + 1][1][sum - vals[idx]];
    }

    if(dp2[idx + 1][1][sum] == -1) {
      dp2[idx + 1][1][sum] = rec(vals, idx + 1, sum);
    }
    v2 = dp2[idx + 1][1][sum];

    if(v1 == 0 || v2 == 0) {
      val = (v1 == 0) ? v1 : v2;
      return val;
    }
    if(v1 > 0 && v2 > 0) {
      val = INT_MAX;
      return val;
    }
    if(v1 < 0 && v2 < 0) {
      val = max(v1, v2);
      return val;
    } else {
      val = min(v1, v2);
      return val;  
    }

    return val;
  }
}; 
