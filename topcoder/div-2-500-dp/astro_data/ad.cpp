#include <iostream>
#include <vector>
#include <algorithm> 
#include <climits>
using namespace std;

#define E 55

char dp[E][E];

void fill_dp(int val) {
  for(int i = 0; i < E; i++) {
    for(int j = 0; j < E; j++) {
      dp[i][j] = -1;
    }
  }
}

class AstronomicalRecordsEasy {
public : 
  int minimalPlanets(vector<int> a, vector<int> b) {
    fill_dp(-1);

    int minp = INT_MAX;
    for(int i = 0; i < b.size(); i++) {
      minp = min(minp, desc_count(a, b, i));
    }

    return minp;
  }
private : 
  int desc_count(vector<int> a, vector<int> b, int idx) {
    for(int i = 0; i < a.size(); i++) {
    }
  } 
};

