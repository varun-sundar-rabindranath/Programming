#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

#define MAX_N 55

double dp[MAX_N][MAX_N][2];

void fill_dp(double val) {
  for(int i = 0; i < MAX_N; i++) {
    for(int j = 0; j < MAX_N; j++) {
      for(int k = 0; k < 2; k++) {
	dp[i][j][k] = val;
      }
    }
  }
}



class FoxPlayingGame {
public : 
  double theMax(int na, int nb, int pa, int pb) {
    fill_dp(0);
    double dpa = (double)pa;
    double dpb = (double)pb;
    double div_val = 1000;
    double ascore = dpa / div_val;
    double bscore = dpb / div_val;

    /* Fill first row/col */
    dp[0][0][0] = 0;
    dp[0][0][1] = 0;
    for(int i = 0; i < MAX_N; i++) {
      dp[0][i][0] = i * ascore;
      dp[0][i][1] = i * ascore;
    }

    for(int i = 1; i <= nb; i++) {
      for(int j = 1; j <= na; j++) {

		double v1 = dp[i - 1][j][0] * bscore;
		double v2 = dp[i - 1][j][1] * bscore;
		double v3 = dp[i][j - 1][0] + ascore;
		double v4 = dp[i][j - 1][1] + ascore;

		dp[i][j][0] = max(max(v1, v2), max(v3, v4));
		dp[i][j][1] = min(min(v1, v2), min(v3, v4));

      }
    }

    return dp[nb][na][0];
  }
};
