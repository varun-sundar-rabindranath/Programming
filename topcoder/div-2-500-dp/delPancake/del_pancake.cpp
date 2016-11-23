#include <iostream>
#include <vector>

using namespace std;

#define MAX_N 15
#define MAX_DEL 105

double dp[MAX_N];

void init_dp(double val) {
  for(int i = 0; i < MAX_N; i++) {
      dp[i] = val;
  }
}

class RandomPancakeStackDiv2
{
public:
    double expectedDeliciousness(vector <int> d)
    {
      init_dp(-1);
      dp[0] = 1;
      return rec(d, d.size(), 0);
    }
private:

    double rec(vector<int> d, int base, double del) {

      double prob = 0;
      double e = (double)base;
      for(int i = 0; i < base; i++) {
	prob += (del + (double)d[i]) * 

      }
    }
#if 0
    double rec(vector<int> &d, int base) {

      if(dp[base] == -1) {
	double prob = 0;
	int num_elements = base;
      	for(int i = 0; i < base; i++) {
      	  double this_val =  (((double)d[i] * rec(d, i)) / (double)num_elements);
	  prob += this_val;
	  cout<<"base "<<base<<" this_val "<<this_val<<endl;
      	}
	dp[base] = prob;
      }

      return dp[base];
    }
#endif
};

int main() {
  
  RandomPancakeStackDiv2 pan;
  vector<int> d;
  d.push_back(1);
  d.push_back(1);
  d.push_back(1);
  cout<<pan.expectedDeliciousness(d)<<endl;

  for(int i = 0; i <= d.size(); i++) {
    cout<<dp[i]<<" ";
  }
  cout<<endl;

  return 0;
}


