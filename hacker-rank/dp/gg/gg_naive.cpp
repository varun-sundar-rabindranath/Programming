#include <iostream>
#include <vector>
#include <string>
#include <cassert>

#define ULL unsigned long long int
#define REP(var, n) for(int var = 0; var < (n); var++)
#define REP_1(var, n) for(int var = 1; var < (n); var++)

ULL dp[3005][3005];

void fill_dp(ULL val) {
  REP(i, 3005)
    REP(j, 3005)
      dp[i][j] = val;
}

using namespace std;

ULL M;

vector<int> swap(vector<int> s, int i, int j) {
  assert(i >= 0);
  assert(j < s.size());

  int c = s[i];
  s[i] = s[j];
  s[j] = c;

  return s;
}

int naive_cost = 0;

void permute(vector<int> s, string &ts, int idx) {

  for(int i = idx; i < s.size(); i++){
    permute(swap(s, idx, i), ts, idx + 1);
  }

  if(idx == s.size()) {

    bool print_it = true;
    for(int j = 0; j < ts.length(); j++) {
      if(ts[j] == 'G' && s[j] < s[j + 1]) {
	print_it = false;
	break;
      }
      if(ts[j] == 'L' && s[j] > s[j + 1]) {
	print_it = false;
	break;
      }	
    }

    if(print_it) {
      naive_cost++;
#if 0
      for(int i = 0; i < s.size(); i++)
	cout<<s[i]<<" ";
      cout<<endl;
#endif
    }
  }

}

ULL prefix_sum[3005];

ULL solve(string test_string, int valid_n) {
  int ways = 1;
  int n = 1;

  for(int i = 0; i < test_string.length(); i++) {

    n++;

    ULL running_sum  = 0;
    for(int j = 0; j < n; j++) {
      running_sum = ((running_sum % M) + (dp[j][i] % M)) % M;
      prefix_sum[j] = running_sum;
    }

    for(int j = 0; j < n; j++) {
      int left_out = j;
      ULL cost = 0;

      if(test_string[i] == 'G') {

#if 1
        if(left_out < n - 1) {
            dp[left_out][i + 1] = ( ((prefix_sum[n - 2]  - prefix_sum[left_out] + M) % M) + (dp[left_out][i] % M) ) % M;
            //dp[left_out][i + 1] = ( (dp[left_out][i + 1] % M) + (dp[left_out][i] % M) ) % M;
        } else {
            dp[left_out][i + 1] = 0;
        }
#endif
//	ULL v1 = dp[left_out][i + 1];

#if 0
	for(int k = left_out; k < n - 1; k++) {
	  cost = ((cost % M) + (dp[k][i] % M)) % M;
	}
	dp[left_out][i + 1] = cost;
#endif
//	ULL v2 = dp[left_out][i + 1];

#if 0
	if(v1 != v2) {
	  /* Print dp */
	  for(int k = 0; k < n; k++) {
	    cout<<k<<" : "<<dp[k][i]<<" - "<<prefix_sum[k]<<endl;
    	  }
	  cout<<endl;

	  /* Print v1 and v2 */
	  cout<<"v1 -> "<<v1<<" | "<<"v2 -> "<<v2<<endl;
	  cout<<"leftout -> "<<left_out<<" | "<<" n - 2 -> "<<n-2<<endl;
	  return 0;
	}
#endif
      }

      if(test_string[i] == 'L') {
          if(0 < left_out) {
            dp[left_out][i + 1] = ( (prefix_sum[left_out - 1] % M) - (prefix_sum[0] % M) + (dp[0][i] % M) ) % M;
            //dp[left_out][i + 1] = ( (dp[left_out][i + 1] % M) + (dp[0][i] % M) ) % M;
          } else {
            dp[left_out][i + 1] = 0;
          }
#if 0
	for(int k = 0; k < left_out; k++) {
	  cost =  ((cost % M) + (dp[k][i] % M)) % M;
	}
	dp[left_out][i + 1] = cost;
#endif
      }

    }

  }


  ULL ret_val = 0;
  for(int i = 0; i <= test_string.length(); i++) {
    ret_val = ((ret_val % M) + (dp[i][test_string.length()] % M)) % M;
  }
  
 
  return ret_val;

}

int main() {
#if 0
  vector<int> test;
  for(int i = 0; i < 10; i++) 
    test.push_back(i);

  string test_string = "LLGGLLGLG";

  permute(test, test_string, 0);
  cout<<"Naive cost - "<<naive_cost<<endl;
#endif

  int N;
  cin>>N;
  cin>>M;
  string test_string;
  cin>>test_string;

  /* Use dp */
  fill_dp(0);
  dp[0][0] = 1;
  cout<<solve(test_string, test_string.length())<<endl;

  return 0;
}
