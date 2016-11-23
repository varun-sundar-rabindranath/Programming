#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#define MAXDIFF 1500

char dp[MAXDIFF][MAXDIFF];

void init_dp(int val) {
  for(int i = 0; i < MAXDIFF; i++) {
    for(int j = 0; j < MAXDIFF; j++) {
      dp[i][j] = val;
    }
  }
}

class BearPlaysDiv2
{
public:
    string equalPiles(int A, int B, int C)
    {
	init_dp(-1);
	int d1 = abs(A - B), d2 = abs(B - C);

	if(d1 == 0 && d2 == 0) { return "possible"; }

	rec(A, B, C, abs(A - B), abs(B - C));

	if(dp[0][0] == -1) { return "impossible"; }
	else { return "possible"; }
    }

private:
    void rec(int A, int B, int C, int d1, int d2) {
      if(dp[0][0] == 1) { return; }
      if(dp[d1][d2] == 1) { return; }
      dp[d1][d2] = 1;

      /* Option 1 (select A pile and B pile) */
      int A1 = A, B1 = B, C1 = C, D11, D21;
      if(A1 > B1) { A1 -= B1; B1 += B1; }
      else	  { B1 -= A1; A1 += A1; }
      D11 = abs(A1 - B1); D21 = abs(B1 - C1);
      if(D11 == 0 && D21 == 0) { dp[D11][D21] = 1; return; }
      rec(A1, B1, C1, D11, D21);

      /* Option 2 (select A pile and C pile) */
      int A2 = A, B2 = B, C2 = C, D12, D22; 
      if(A2 > C2) { A2 -= C2; C2 += C2; }
      else	  { C2 -= A2; A2 += A2; }
      D12 = abs(A2 - B2); D22 = abs(B2 - C2);
      if(D12 == 0 && D22 == 0) { dp[D12][D22] = 1; return; }
      rec(A2, B2, C2, D12, D22);

      /* Option 3 (select B and C pile) */
      int A3 = A, B3 = B, C3 = C, D13, D23;
      if(B3 > C3) { B3 -= C3; C3 += C3; }
      else	  { C3 -= B3; B3 += B3; }
      D13 = abs(A3 - B3); D23 = abs(B3 - C3);
      if(D13 == 0 && D23 == 0) { dp[D13][D23] = 1; return; }
      rec(A3, B3, C3, D13, D23);

      return;
    }
    
};

int main() {
  BearPlaysDiv2 bp;
  cout<<bp.equalPiles(10, 15, 35)<<endl;
  cout<<bp.equalPiles(4, 6, 8)<<endl;
  cout<<bp.equalPiles(18, 18, 18)<<endl;
  cout<<bp.equalPiles(1, 1, 2)<<endl;
  cout<<bp.equalPiles(225, 500, 475)<<endl;
  return 0;
}
