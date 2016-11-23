#include <iostream>
#include <string> 
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;

#define SLEN 55
#define FILL50 0x3FFFFFFFFFFFF

#define bitpos(val, idx) (val & (1 << idx))

long long int get_left_out(long long int val, int slen, int pos1, int pos2) {

  pos1 = slen - pos1 - 1;
  pos2 = slen - pos2 - 1;

  int mask = 0;
  for(int i = pos1; i >= pos2; i--) {
    mask |= (1 << i);
  }


  if(bitpos(val, pos1) == 0) {
    return 2;
  } else {
    return (val & mask) >> pos2;
  }
}

bool check_pow_5(long long int val, vector<long long int> &pow_5) {
  return (binary_search(pow_5.begin(), pow_5.end(), val));
}

long long int get_i_from_s(string s) {
    int rv = 0;
    /* make input bits */
    for(int i = 0; i < s.length(); i++) {
      if(s[i] == '1') {
	rv |= (1 << (s.length() - 1 - i));
      }
    }

    return rv;
}

int dp[SLEN];

void fill_dp(int val) {
  for(int i = 0; i < SLEN; i++) {
    dp[i] = val;
  }
}

class CuttingBitString {
public :
  int getmin(string s) {

    fill_dp(INT_MAX);

    long long int ip = 0;

    ip = get_i_from_s(s);

    vector<long long int> pow_5;

    long long int powers = 1;

    long long int all50 = 0;
    for(int i = 0; i < 50; i++) {
      long long int v = 1;
      all50 |= (v << i);
    }

    while(powers <= all50) {
      pow_5.push_back(powers);
      powers *= 5;
    }

    if(bitpos(ip, s.length() - 1) == 0) { 
      dp[0] = -1; 
    } else {
      dp[0] = 1;
    }

    //cout<<"leftout trial "<<get_left_out(ip, s.length(), 1, 1)<<endl;
    //cout<<"leftout trial "<<get_left_out(ip, s.length(), 1, 2)<<endl;
    //cout<<"leftout trial "<<get_left_out(ip, s.length(), 0, 2)<<endl;
    
    for(int i = 1; i < s.length(); i++) {
      vector<int> valid_pos;
      /* locate prev valid bitstring */
      for(int j = i - 1; j >= 0; j--) {
	if(dp[j] != -1) { valid_pos.push_back(j); }
      }

      for(int j = 0; j < valid_pos.size(); j++) {
	long long int left_out = get_left_out(ip, s.length(), valid_pos[j] + 1, i);
	if(check_pow_5(left_out, pow_5)) { dp[i] = min(dp[i], dp[ valid_pos[j] ] + 1); }
      }
      
      if(check_pow_5( get_left_out(ip, s.length(), 0, i), pow_5)) { dp[i] = min(dp[i], 1); }

      if(dp[i] == INT_MAX) { dp[i] = -1; }

    }

#if 1
    for(int i = 0; i < s.length(); i++) {
      cout<<dp[i]<<" ";
    }
    cout<<endl;
#endif


    return (dp[s.length() - 1]);

  }
};

int main() {
  CuttingBitString c;
  cout<<c.getmin("0101")<<endl;
  return 0;
}
