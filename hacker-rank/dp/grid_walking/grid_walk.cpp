#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

#define MAXN 10
#define MAXM 300
#define MAXD 100
#define MOD 1000000007

vector<int> position;
vector<int> dim;

long long int dp[MAXN][MAXD][MAXM];

void set_dp(long long int val) {
  for(int k = 0; k < MAXN; k++) {
    for(int i = 0; i < MAXD; i++) {
      for(int j = 0; j < MAXM; j++) {
        dp[k][i][j] = -1;
      }
    }
  }
}

long long int rec(vector<int> pos, int steps) {

  assert(pos.size() == dim.size());

  for(int i = 0; i < pos.size(); i++) {
    if(pos[i] <= 0 || pos[i] > dim[i]) {
      return 0;
    }
  }

  if(steps == 0) { return 1; }

  /* Move one step in all directions */
  int n_dim = dim.size();

  long long int ways = 0;

  for(int i = 0; i < n_dim; i++) {
    vector<int> back = pos;
    vector<int> front = pos;
    back[i] = pos[i] - 1;
    front[i] = pos[i] + 1;
    /* One step back */
    ways += rec(back, steps - 1);
    /* One step front */
    ways += rec(front, steps - 1);
  }

  return ways;
}

long long int rec_1d(int curr_dim, int curr_pos, int steps, int dim_extent) {
  if(curr_pos > dim_extent || curr_pos <= 0) {
    return 0;
  }

  if(steps == 0) {
    return 1;
  }


  long long int move_fwd, move_bck;

  if(dp[curr_dim][curr_pos + 1][steps - 1] == -1) {
    move_fwd = rec_1d(curr_dim, curr_pos + 1, steps - 1, dim_extent) % MOD;
  } else {
    move_fwd = dp[curr_dim][curr_pos + 1][steps - 1];
  }

  if(dp[curr_dim][curr_pos - 1][steps - 1] == -1) {
    move_bck = rec_1d(curr_dim, curr_pos - 1, steps - 1, dim_extent) % MOD;
  } else {
    move_bck = dp[curr_dim][curr_pos - 1][steps - 1];
  }

  dp[curr_dim][curr_pos][steps] = (move_fwd + move_bck) % MOD;

  return (move_fwd + move_bck) % MOD;
}

int main() {

  int T;
  cin>>T;

  while(T--) {
    int N, M;
    cin>>N;
    cin>>M;

    set_dp(-1);
    position.clear();
    dim.clear();

    for(int i = 0; i < N; i++) {
      int pos;
      cin>>pos;
      position.push_back(pos);
    }

    for(int i = 0; i < N; i++) {
      int d;
      cin>>d;
      dim.push_back(d);
    }

    for(int i = 0; i < N; i++) {
      rec_1d(i, position[i], M, dim[i]);
    }

    cout<<dp[0][position[0]][M]<<endl;
  }


  return 0;
}
