#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

class NoRepeatPlaylist {
public :
  int numPlaylists(int N, int M, int P) {
    /* Should fill the first m + 1 positions 
     * with N, N - 1, N - 2 and so on
     */
    long long int init_m_ways = 1;
    for(int i = 0; i <= M + 1; i++) {
      init_m_ways *= (N - i);
    }


    long long int rest_permute_ways = 1;
    for(int i = M + 2; i < N; i++) {
      rest_permute_ways *= (N - i);
    }



  }

private :
  long long int ncr(int n, int r) {
    assert(n >= r);
    return (factorial(n) / (factorial(n - r) * factorial(r)) );
  }
  long long int factorial(int n) {
    long long int val = 1;
    for(int i = 1; i <= n; i++) {
      val *= i;
    }
    return val;
  }
};
