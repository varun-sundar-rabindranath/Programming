#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
using namespace std;

int get_divisor(int n) {
  assert(n != 1);
  if(n == 2) { return -1; }
  int div = -1;
  int sq_root = sqrt(n) + 1;
  for(int i = 2; i <= sq_root; i++) {	
    if(n % i == 0) {
      div = i;
    }
  }
  return div;
}
void prime_factors(vector<int> &factors, int n) {
  if(n == 1) { return; }
  assert(n != 0);
  int div = get_divisor(n);
  if(div == -1) {
    factors.push_back(n);
  } else {
    prime_factors(factors, div);
    prime_factors(factors, n / div);
  }

}

int main() {
  vector<int> ans;
  prime_factors(ans, 77777);

  for(int i = 0; i < ans.size(); i++) {
    cout<<ans[i]<<" ";
  }
  return 0;
}
