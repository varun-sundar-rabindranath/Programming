#include <iostream>
#include <vector>
#include <algorithm> 
using namespace std;

class PalindromicSubstringsDiv2
{
public:

    int count(vector <string> S1, vector <string> S2)
    {

      vector<vector<int> > alpha_pos;
      vector<vector<int> > contrib_palins;
      alpha_pos.resize(26);
      

      string s = "";
      for(int i = 0; i < S1.size(); i++) {
	s += S1[i];
      }
      for(int i = 0; i < S2.size(); i++) {
	s += S2[i];
      }
      
      int num_palin = 0;

      contrib_palins.resize(s.size());

      for(int i = 0; i < s.size(); i++) {
	int alpha_int = s[i] - 97;
	for(unsigned int j = 0; j < alpha_pos[alpha_int].size(); j++) {
	  int curr_pos = alpha_pos[alpha_int][j];
	  if(i - curr_pos == 1) {
	    num_palin++;
	    contrib_palins[i].push_back(2);
	  } else if(i - curr_pos == 2) {
	    num_palin++;
	    contrib_palins[i].push_back(3);
	  } else {
	    int space = i - curr_pos;
	    int to_search = space - 1;
	    bool element_present = false;
	  
#if 0
	    for(int k = 0; k < contrib_palins[i - 1].size(); k++) {
	      if(contrib_palins[i - 1][k] == to_search) { element_present = true; break; }
	    }
#endif

	    if(binary_search(contrib_palins[i - 1].begin(), contrib_palins[i - 1].end(), to_search)) {
	      num_palin++;
	      contrib_palins[i].push_back(to_search + 2);
	    }
	  }
	}  

	/* Account for single letter palindrome */
	num_palin++;
	contrib_palins[i].push_back(1);

	sort(contrib_palins[i].begin(), contrib_palins[i].end());

	alpha_pos[alpha_int].push_back(i);
      }

#if 0
      for(int i = 0; i < contrib_palins.size(); i++) {
	cout<<"Pos "<<i<<" : ";
	for(int j = 0; j < contrib_palins[i].size(); j++) {
	  cout<<contrib_palins[i][j]<<" ";
	}
	cout<<endl;
      }
#endif

      return num_palin;
    }
};

int main() {
  PalindromicSubstringsDiv2 p;
  vector<string> s1;
  vector<string> s2;

  s1.push_back("abccba");
//  s1.push_back("a");
//  s1.push_back("");

  s2.push_back("");

  cout<<p.count(s1, s2)<<endl;

  return 0;

}
