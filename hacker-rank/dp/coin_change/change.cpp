#include <iostream>
#include <vector>
using namespace std;

#define MAXM 55
#define MAXN 255

long long int dp[MAXM][MAXN];

long long int rec(vector<int> &elements, int e_s, int sum) {
  if(sum == 0) { return 1; }
  if(sum < 0) { return 0; }
  if(e_s <= 0) { return 0; }

#if 1

  if(dp[e_s][sum] == -1) {
    long long int ret_val = (rec(elements, e_s - 1, sum) + rec(elements, e_s, sum - elements[e_s - 1]));
    dp[e_s][sum] = ret_val;
    return ret_val;
  } else {
    return dp[e_s][sum];
  }

#if 0
  long long int wo_consider;
  long long int consider;

  if(dp[e_s -1][sum] == -1) {
    wo_consider = rec(elements, e_s - 1, sum);
    dp[e_s - 1][sum] = wo_consider;
  } else {
    wo_consider = dp[e_s - 1][sum];
  }

  if(dp[e_s][sum - elements[e_s - 1]] == -1) {
    consider = rec(elements, e_s, sum - elements[e_s - 1]);
    dp[e_s][sum - elements[e_s - 1]] = consider;
  } else {
    consider = dp[e_s][sum - elements[e_s - 1]];
  }

  return (wo_consider + consider);
#endif
#else

  return (rec(elements, e_s - 1, sum) + rec(elements, e_s, sum - elements[e_s - 1]));
#endif

}

int main() {
  int N, M;
  cin>>N;
  cin>>M;
  
  vector<int> elements;

  /* Make dp 0 */
  for(int i = 0; i < MAXM; i++) {
    for(int j = 0; j < MAXN; j++) {
      dp[i][j] = -1;
    }
  }

  for(int i = 0; i < M; i++) {
    int element;
    cin>>element;
    elements.push_back(element);
  }

  cout<<rec(elements, M, N)<<endl;

  return 0;
}
