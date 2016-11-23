#include <iostream>
#include <vector>
#include <climits>
using namespace std;
#include <stdio.h>
#include <cassert>

int main() {
  int T, N;

  cin>>T;

  while(T--) {
    int n;
    cin>>n;

    vector<long long int> arr;
    for(int i = 0; i < n; i++) {
      int element;
      cin>>element;
      arr.push_back(element);
    }

    vector<long long int> new_arr;

    /* group all negs and pos */
    long long int sum = arr[0];
    for(int i = 1; i < n; i++) {
      if((arr[i - 1] < 0 && arr[i] < 0) || (arr[i - 1] >= 0 && arr[i] >= 0)) {
	sum += arr[i];
      } else {
	new_arr.push_back(sum);
	sum = arr[i];
      }
    }

    { new_arr.push_back(sum); }

    assert(new_arr.size() != 0);

    if(new_arr.size() == 1) {
      if(new_arr[0] < 0) {
	/* Find the max among all elemetns */
	long long int max_val = LLONG_MIN;
	for(int i = 0; i < arr.size(); i++) {
	  max_val = max(max_val, arr[i]);
	}
	cout<<max_val<<" "<<max_val<<endl; 
      }	 else {
	/* All positive */
	cout<<new_arr[0]<<" "<<new_arr[0]<<endl;
      }
      continue;
    }

    int start_pos = 0;
    /* Start off with a positive part */
    if(new_arr[0] < 0) { start_pos = 1; }


    vector<long long int> dp;
    dp.resize(new_arr.size());

    for(int i = 0; i < dp.size(); i++) {
      dp[i] = LLONG_MIN;
    }

    dp[start_pos] = new_arr[start_pos];
    for(int i = start_pos + 2; i < new_arr.size(); i = i + 2) {
      dp[i] = max(dp[i - 2] + new_arr[i - 1] + new_arr[i], 
		  new_arr[i]);
      
    }

    /* find max in dp and print */
    long long int max_val = LLONG_MIN;
    for(int i = 0; i < dp.size(); i++) {
      max_val = max(max_val, dp[i]);
    }

    cout<<max_val<<" ";

    long long int ret_sum = 0;
    /* find sum of all pos val in new arr */
    for(int i = 0; i < new_arr.size(); i++) {
      if(new_arr[i] > 0) {
	ret_sum += new_arr[i];
      }
    }

    cout<<ret_sum<<endl;

  }

  return 0;
}
