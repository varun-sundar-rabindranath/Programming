#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

#define MAX_N 25
#define MAX_E 100005

char dp[MAX_N * MAX_E];

void init_dp(int val) {
  for(int i = 0; i < MAX_E * MAX_N; i++) {
    dp[i] = val;
  }
}

class NumbersChallenge
{
public:
 int MinNumber(vector <int> S)
   {
	init_dp(-1);
       
	dp[0] = 1;

	vector<int> sums;
	sums.push_back(0);

	for(int i = 0; i < S.size(); i++) {

	    int element = S[i];
	    int num_e = sums.size();
	    for(int j = 0; j < num_e; j++) {
	      if(dp[ element + sums[j] ] == -1) {
		sums.push_back(element + sums[j]);
		dp[element + sums[j]] = 1;
	      }
	    }
	}

	bool min_found = false;
	int minval = -1;
	int iter = 0;

	while(!min_found) {
	  if(dp[iter] == -1) {
	    min_found = true;
	    minval = iter;
	  }
	  iter++;
	}

	return minval;
   }
};


