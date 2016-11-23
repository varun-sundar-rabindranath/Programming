#include <iostream>
#include <vector>
#include <string> 
#include <algorithm>
#include <cassert> 
using namespace std;

int dp[1005][1005];
int dir[1005][1005];

void fill_dp(int val) {
  for(int i = 0; i < 1005; i++) {
    for(int j = 0; j < 1005; j++) {
      dp[i][j] = val;
    }
  }
}
void fill_dir(int val) {
  for(int i = 0; i < 1005; i++) {
    for(int j = 0; j < 1005; j++) {
      dir[i][j] = val;
    }
  }
}



#define DOWN  1
#define LEFT  2
#define UP    3
#define UP_LEFT 4

string string_reverse(string s) {
  string rev = s;
  for(int i = 0; i < s.length() / 2; i++) {
    char c1 = s[i];
    char c2 = s[s.length() - 1 - i];
    rev[s.length() - 1 - i] = c1;
    rev[i] = c2;
  }
  return rev;
}

int longest_palin(string seq1, string seq2, string &result) {
  fill_dp(0);
  fill_dir(0);
  
  int max = -1;
  int max_i = -1;
  int max_j = -1;

  for(int i = 1; i <= seq1.length(); i++) {
    for(int j = 1; j <= seq2.length(); j++) {
      if(seq1[j - 1]  == seq2[i - 1]) {
	dp[i][j] = dp[i - 1][j - 1] + 1;
	dir[i][j] = UP_LEFT;
      } else {
	if(dp[i - 1][j] >= dp[i][j - 1]) {
	  dp[i][j] = dp[i - 1][j];
	  dir[i][j] = UP;	  
	} else {
	  dp[i][j] = dp[i][j - 1];
	  dir[i][j] = LEFT;
	}
      }	

      if(dp[i][j] > max) {
	max = dp[i][j]; //access(longest, i, j);
	max_i = i;
	max_j = j;
      }
    }
  }

  int iter_i = max_i;
  int iter_j = max_j;

  string tmp = "";

  while(iter_i >= 0 && iter_j >=0) {
    switch(dir[iter_i][iter_j]) {
      case UP_LEFT : tmp += (seq2[iter_i - 1]); //cout<<seq2[iter_i - 1]<<" ";
		     iter_i -= 1;
		     iter_j -= 1;
		     break;
      case LEFT : iter_j -= 1;
		  break;
      case UP : iter_i -= 1; 
		break;
      case 0 : iter_i -= 1;
	       iter_j -= 1;
	       break;
      default : 
	       assert(0);
		    
    }
  }

  result = string_reverse(tmp);

  return dp[seq1.length()][seq2.length()];
}


int main() {

  int T;
  cin>>T;
  
  while(T) {
    string s1;
    string s2;

    string result1;
    string result2;

    cin>>s1;
    cin>>s2;

    fill_dp( -1 );
    int l1 = longest_palin(s1, string_reverse(s1), result1);
    assert(l1 == result1.length());
    //cout<<"s1 length - "<<s1.length()<<" | "<<l1<<endl;
    fill_dp( -1 );
    int l2 = longest_palin(s2, string_reverse(s2), result2);
    assert(l2 == result2.length());
    //cout<<"s2 length - "<<s2.length()<<" | "<<l2<<endl;
    
    if(l1 % 2 == 1 && l2 % 2 == 1) {
      char s1_mid_char = result1[result1.length() / 2];
      char s2_mid_char = result2[result2.length() / 2];
      
      cout<<l1<<" | "<<l2<<" - ";
      cout<<endl<<result1<<endl<<result2<<endl;

      if(s1_mid_char == s2_mid_char) {
	cout<<l1 + l2<<endl;
      } else {
	cout<<l1 + l2 - 1<<endl;
      }
    } else {
      cout<<l1 + l2<<endl;
    }
    T--;
  }

  return 0;
}
