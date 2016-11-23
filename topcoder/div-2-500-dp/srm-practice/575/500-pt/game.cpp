#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stack>
using namespace std;

#define MAX_N 1005

int can_reach[MAX_N][MAX_N];

bool can_win[MAX_N];

void fill_can_reach(int val) {
  for(int i = 0; i < MAX_N; i++) {
    for(int j = 0; j < MAX_N; j++) {
      can_reach[i][j] = val;
    }
  }
}
void fill_can_win(bool val) {
  for(int i = 0; i < MAX_N; i++) {
    can_win[i] = val;
  }
}

vector<int> get_div(int n) {
  vector<int> ret;
  for(int i = 2; i < n; i++) {
    if(n % i == 0) { ret.push_back(i); }
  }
  return ret;
}

void update_can_win(int p, int move) {
 if(can_win[p]) { return; }
 if(move % 2 == 0) {
   can_win[p] = true;
 } 

 /* Previous states */
 int N = p;
 vector<int> div = get_div(N);
 for(int i = 0; i < div.size(); i++) {
   update_can_win( (N / div[i]) + N, move + 1); 
 }
}



class TheNumberGameDivTwo {
public : 
  string find(int n) {
    if(n <= 3) { return "Brus"; }
    fill_can_reach(-1);
    fill_can_win(false);

    vector<int> primes = get_primes(n);

    for(int i = 0; i < primes.size(); i++) {
      update_can_win(primes[i] * 2, 0);
    }

    if(can_win[n]) {
      return "John";
    } else {
      return "Brus";
    }

  }
private :
  vector<int> get_primes(int n) {
    vector<int> ret;

    if(n == 1) { return ret; }
    if(n == 2) { ret.push_back(2); return ret; }

    for(int i = 3; i <= n; i++) {
      /* Check for div till square root of n */
      bool div_found = false;
      int range = sqrt(i) + 1;
      for(int j = 2; j <= range; j++) {
	if(i % j == 0) {
	  div_found = true;
	  break;
	}
      }
      if(!div_found) {
	ret.push_back(i);
      }
    }
    return ret;
  }
};
