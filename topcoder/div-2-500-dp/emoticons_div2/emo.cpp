#include<iostream>
#include <vector>

using namespace std;

#define INF 50000
#define MAXS 1005

int dp[MAXS][MAXS];

void init_dp() {
  for(int i = 0; i < MAXS; i++) {
    for(int j = 0; j < MAXS; j++) {
      dp[i][j] = -1;
    }
  }
}

class EmoticonsDiv2 {
public : 
  int printSmiles(int n) {
    init_dp();
    return rec(n - 1, 1, 0);
  } 
private :
  int rec(int need, int already_in, int cb) {
    if(need == 0) { return 0; } 
    if(need < 0) { return INF; }

    int cost1 = INF, cost2 = INF;
    /* do a copy and paste */
    if(need - already_in >= 0) {
      if(dp[need - already_in][already_in] == -1) {
        dp[need - already_in][already_in] = rec(need - already_in, already_in * 2, already_in) + 2;
      }
      cost1 = dp[need - already_in][already_in]; //rec(need - already_in, already_in * 2, already_in) + 2;
    } else {
      cost1 = INF;
    }
    /* do a paste alone */
    if(cb != 0) {
      cost2 = rec(need - cb, already_in + cb, cb) + 1;
    }
    return min(cost1, cost2);
  }
};

int main() {
  EmoticonsDiv2 e;
  cout<<e.printSmiles(2)<<endl;
  cout<<e.printSmiles(6)<<endl;
  cout<<e.printSmiles(11)<<endl;
  cout<<e.printSmiles(16)<<endl;
  cout<<e.printSmiles(1000)<<endl;
  return 0;
}
