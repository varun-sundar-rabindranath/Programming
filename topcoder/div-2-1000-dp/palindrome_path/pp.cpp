#include <iostream>
#include <vector>
#include <string>
#include <climits>

using namespace std;

vector<vector<pair<int, char> > > graph;

#define MAX_N 25

int dp[MAX_N][MAX_N];

void fill_dp(int val) {
  for(int i = 0; i < MAX_N; i++) {
    for(int j = 0; j < MAX_N; j++) {
      dp[i][j] = val;
      if(i == j) {
	dp[i][j] = 0;
      }
    }
  }
}

class PalindromePath {
public :
  int shortestLength(int n, vector<int> a, vector<int> b, vector<int> c) {

    fill_dp(-1);

    graph.resize(n);
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
	graph[i].push_back(pair<int, char>(-1, '-'));
      }
    }

    for(int i = 0; i < a.size(); i++) {
      graph[a[i]][b[i]].first = 1;
      graph[a[i]][b[i]].second = c[i];
      graph[b[i]][a[i]].first = 1;
      graph[b[i]][a[i]].second = c[i];
      dp[a[i]][b[i]] = 1;
      dp[b[i]][a[i]] = 1;
    }

    if(dp[0][1] != -1) { return dp[0][1]; }
    dp[0][1] = -2;
    return rec(0, 1, n);

  }

  int rec(int ln, int rn, int n) {

    if(dp[ln][rn] >= 0) { return dp[ln][rn]; }

    /* pairs to check */
    vector<pair<int, int> > to_check;

    /* Labels leading away from ln */
    for(int i = 0; i < n; i++) {
      if(graph[ln][i].first == -1) { continue; }
      char l1 = graph[ln][i].second;
      /* Do we have this label from rn */
      for(int j = 0; j < n; j++) {
	if(graph[rn][j].second == l1) {
	  if(dp[i][j] == -2) { continue; }
	  to_check.push_back(pair<int, int> (i, j));
	}
      }
    } 

    int min_val = INT_MAX;

    for(int i = 0; i < to_check.size(); i++) {
      int new_ln = to_check[i].first;
      int new_rn = to_check[i].second;
      if(dp[new_ln][new_rn] == -1) {
	dp[new_ln][new_rn] = -2;
	dp[new_ln][new_rn] = rec(new_ln, new_rn, n);
      }
    }

    for(int i = 0; i < to_check.size(); i++) {
      min_val = min(min_val, dp[to_check[i].first][to_check[i].second]);
    }

    if(min_val == -1) { min_val = INT_MAX; }

    if(min_val == INT_MAX) {
      dp[ln][rn] = -1;
    } else {
      dp[ln][rn] = min_val + 2;
    }

    return dp[ln][rn];

  }
};
