#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
using namespace std;

#define MAXN 5005

int initmap[MAXN];

void init_initmap() {
  for(int i = 0; i < MAXN; i++) {
    initmap[i] = i;
  }
}

void correct_the_init_map(int to_eliminate, int valid_n) {

  bool eliminate_seen = false;

  for(int i = 0; i < valid_n; i++) {
    if(eliminate_seen && initmap[i] != -1) {
      initmap[i] -= 1;
    }
    if(!eliminate_seen) {
      if(initmap[i] == to_eliminate) {
        eliminate_seen = true;
        initmap[i] = -1;
      }
    }
  }

  assert(eliminate_seen);
}

class ChooseTheBestOne {
public : 
  int countNumber(int N) {
    init_initmap();

    int num_elements_available = N;
    int last_eliminated = 0;
    for(int i = 1; i < N; i++) {
      int threshold = pow(i, 3);
      int v1, v2;
      v1 = max(threshold, num_elements_available);
      v2 = min(threshold, num_elements_available);
      int to_eliminate = v1 % v2;
      correct_the_init_map(to_eliminate + last_eliminated, N); 
      num_elements_available--;
    } 

    assert(num_elements_available == 1);

    int winner = -1;
    for(int i = 0; i < N; i++) {
      if(initmap[i] != -1) {
	winner = i + 1;
	break;
      }
    }

    assert(winner != -1);
    return winner;
  }
};

int main() {
  ChooseTheBestOne cbo;
  cout<<cbo.countNumber(3)<<endl;
  cout<<cbo.countNumber(6)<<endl;
  cout<<cbo.countNumber(10)<<endl;
  cout<<cbo.countNumber(1234)<<endl;
  return 0;
}
