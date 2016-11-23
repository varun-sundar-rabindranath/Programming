#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define MOD 1000000007
#define MAX_N 200005


int main() {
  string N;

  cin>>N;

  vector<int> str;
  
  for(int i = 0; i < N.size(); i++) {
    char c = N[i];
    int value = c - 48;
    str.push_back(value);
  }

  long long int prev_sum = str[0];
  long long int total_sum = str[0];

  for(int i = 1; i < str.size(); i++) {
    int curr_val = str[i];
    long long int this_sum = ( ( (((prev_sum % MOD) * 10) % MOD) % MOD ) + ((i + 1) * curr_val)) % MOD;
    total_sum = ((total_sum % MOD) + (this_sum % MOD)) % MOD;
    prev_sum = this_sum;
  }

  cout<<total_sum<<endl;

  return 0;
}
