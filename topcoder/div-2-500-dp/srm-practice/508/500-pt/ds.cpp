#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <cassert>
using namespace std;

int min_move = INT_MAX;
class DivideAndShift {
public : 
  int getLeast(int N, int M) {
    if(N == 1) {
      return 0;
    }
    vector<int> factors;
    prime_factors(factors, N);

    rec(factors, N, M, 0);
    return min_move;
  }

private :

  void rec(vector<int> factors, int N, int M, int move) {
    
    if(move > min_move) {
      return;
    }
    if(M == 1) { min_move = move; return; }

    for(int i = 0; i < factors.size(); i++) {
      vector<int> curr_fac = factors;
      int curr_n, curr_m;
      del_element(curr_fac, factors[i]);
      curr_n = N / factors[i];
      curr_m = (M % curr_n) + 1;

      rec(curr_fac, curr_n, curr_m, move + 1);
    }
    /* Left shift */
    int ls_m = M - 1;
    rec(factors, N, ls_m, move + 1);
    /* Right shift */
    int rs_m = M + 1;
    if(rs_m == N + 1) { rs_m = 1; }
    rec(factors, N, rs_m, move + 1);

  }

  void del_element(vector<int> &vec, int element) {
    vector<int>::iterator it = vec.begin();
    bool element_found = false;
    while(it != vec.end()) {
      if(*it == element) {
	it = vec.erase(it);
	element_found = true;
	break;
      }
      it++;
    }
    assert(element_found);
  }

  int get_divisor(int n) {
    
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
};

int main() {
  DivideAndShift ds;
  min_move = INT_MAX;
  cout<<ds.getLeast(56, 14)<<endl;
  min_move = INT_MAX;
  cout<<ds.getLeast(49, 5)<<endl;
  min_move = INT_MAX;
  cout<<ds.getLeast(256, 7)<<endl;
  min_move = INT_MAX;
  cout<<ds.getLeast(6, 1)<<endl;
  min_move = INT_MAX;
  cout<<ds.getLeast(77777, 11111)<<endl;
  min_move = INT_MAX;

  return 0;

}
