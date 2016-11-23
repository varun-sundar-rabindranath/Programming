#include <iostream>
#include <vector>
using namespace std;

#include <stdio.h>

//#define USE_DP

#define MAX_N 100005
      
/* dp will store the splitters val */
vector<vector<long long int> > dp;
vector<long long int> bricks;
vector<long long int> suffix_sum;
int n;

void set_dp(long long int val) {
  for(int i = 0; i < MAX_N; i++) {
      dp[i].resize(2);
      dp[i][0] = val;
      dp[i][1] = val;
  }
}

#define MY_TURN 0
#define HIS_TURN 1

inline vector<long long int> greatest_vector(vector<long long int> a, vector<long long int> b, vector<long long int> c, int index) {

  long long int max_val = max(max(a[index], b[index]), c[index]);
  if(max_val == a[index]) { return a; }
  else if(max_val == b[index]) { return b; }
  else { return c; }
}

vector<long long int> rec(int pos, bool myTurn) {
  vector<long long int> ret_vector(2);
  if((pos + 3) >= n) {
    long long int val = 0;
    for(int i = pos; i < n; i++) {
      val += bricks[pos];
    }

    if(myTurn) {
      ret_vector[MY_TURN] = val;
      ret_vector[HIS_TURN] = 0;
    } else {
      ret_vector[MY_TURN] = 0;
      ret_vector[HIS_TURN] = val;
    }
    
    return ret_vector;
  }

  vector<long long int> split_1(2);
  vector<long long int> split_2(2);
  vector<long long int> split_3(2);

  vector<long long int> split_1_dp = dp[pos + 1];
  vector<long long int> split_2_dp = dp[pos + 2];
  vector<long long int> split_3_dp = dp[pos + 3];

#ifdef USE__DP
  if(split_1_dp[MY_TURN] != -1 && split_1_dp[HIS_TURN] != -1) {
    if(myTurn) {
      split_1 = split_1_dp;
    } else {
      split_1[MY_TURN] = split_1_dp[HIS_TURN];
      split_1[HIS_TURN] = split_1_dp[MY_TURN];
    }
  } else 
#endif
  { 
    split_1 = rec(pos + 1, !myTurn);
  }

#ifdef USE_DP
  if(split_2_dp[MY_TURN] != -1 && split_2_dp[HIS_TURN] != -1) {
    if(myTurn) {
      split_2 = split_2_dp;
    } else {
      split_2[MY_TURN] = split_2_dp[HIS_TURN];
      split_2[HIS_TURN] = split_2_dp[MY_TURN];
    }
  } else
#endif 
  { 
    split_2 = rec(pos + 2, !myTurn);
  }

#ifdef USE_DP
  if(split_3_dp[MY_TURN] != -1 && split_3_dp[HIS_TURN] != -1) {
    if(myTurn) {
      split_3 = split_3_dp;
    } else {
      split_3[MY_TURN] = split_3_dp[HIS_TURN];
      split_3[HIS_TURN] = split_3_dp[MY_TURN];
    }
  } else
#endif
  { 
    split_3 = rec(pos + 3, !myTurn);
  }

  if(myTurn) {
    ret_vector = (greatest_vector(split_1, split_2, split_3, MY_TURN));
    dp[pos] = ret_vector;

    split_1[MY_TURN] += bricks[pos];
    split_2[MY_TURN] += bricks[pos] + bricks[pos + 1];
    split_3[MY_TURN] += bricks[pos] + bricks[pos + 1] + bricks[pos + 2];

    ret_vector = (greatest_vector(split_1, split_2, split_3, MY_TURN));


  } else {
    split_1[HIS_TURN] += bricks[pos];
    split_2[HIS_TURN] += bricks[pos] + bricks[pos + 1];
    split_3[HIS_TURN] += bricks[pos] + bricks[pos + 1] + bricks[pos + 2];

    ret_vector = (greatest_vector(split_1, split_2, split_3, HIS_TURN));

    dp[pos][MY_TURN] = ret_vector[HIS_TURN];
    dp[pos][HIS_TURN] = ret_vector[MY_TURN];
  }

  return ret_vector;
}

int main() {
  int T, N;
  cin>>T;

  dp.resize(MAX_N);

  while(T--) {
    cin>>n;

    /* Set all dp to -1 */
    set_dp(-1);

    bricks.clear();
    suffix_sum.clear();
    suffix_sum.resize(n);

    for(int i = 0; i < n; i++) {
      long long int brick_val;
      cin>>brick_val;
      bricks.push_back(brick_val);
    }

    suffix_sum[n - 1] = bricks[n - 1];
    for(int i = n - 2; i >= 0; i--) {
      suffix_sum[i] = bricks[i] + suffix_sum[i + 1];
    }

    vector<long long int> res = rec(0, true);
    cout<<res[MY_TURN]<<endl;

#if 0
    cout<<endl;
   for(int j = 0; j < n; j++) {
     cout<<dp[j]<<" ";
   }
#endif

  }
  return 0;
}
