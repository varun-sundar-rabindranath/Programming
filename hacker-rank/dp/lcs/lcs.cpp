#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>

using namespace std;

#define access(ptr, i, j) (ptr[(i) * stride + (j)])

#define DOWN  1
#define LEFT  2
#define UP    3
#define UP_LEFT 4

int main() {
  int N, M;
  cin>>N;
  cin>>M;

  vector<int> seq1, seq2;

  for(int i = 0; i < N; i++) {
    int element;
    cin>>element;
    seq1.push_back(element);
  }

  for(int i = 0; i < M; i++) {
    int element;
    cin>>element;
    seq2.push_back(element);
  }

  int *longest = NULL, *dir = NULL;

  int stride = N + 1;

  longest = (int *)calloc((N + 1) * (M + 1), sizeof(int));
  dir = (int *)calloc((N + 1) * (M + 1), sizeof(int));

  int max = -1;
  int max_i = -1;
  int max_j = -1;
  for(int i = 1; i <= seq2.size(); i++) {
    for(int j = 1; j <= seq1.size(); j++) {
      if(seq1[j - 1]  == seq2[i - 1]) {
	access(longest, i, j) = access(longest, i - 1, j - 1) + 1;
	access(dir, i, j) = UP_LEFT; 
      } else {
	if(access(longest, i - 1, j) >= access(longest, i, j - 1)) {
	  access(longest, i, j) = access(longest, i - 1, j);
	  access(dir, i, j) = UP;
	} else {
	  access(longest, i, j) = access(longest, i, j - 1);
	  access(dir, i, j) = LEFT;
	}
      }	

      if(access(longest, i, j) > max) {
	max = access(longest, i, j);
	max_i = i;
	max_j = j;
      }
    }
  }

  assert(max_i != -1);
  assert(max_j != -1);

  assert(max_i <= M + 1);
  assert(max_j <= N + 1);

  int iter_i = max_i;
  int iter_j = max_j;

  vector<int> res;

  while(iter_i >= 0 && iter_j >=0) {
    switch(access(dir, iter_i, iter_j)) {
      case UP_LEFT : res.push_back(seq2[iter_i - 1]); //cout<<seq2[iter_i - 1]<<" ";
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

  for(int i = res.size() - 1; i >= 0; i--) {
    cout<<res[i]<<" ";
  }
#if 0
  for(int i = 0; i <= seq2.size(); i++) {
    for(int j = 0; j <= seq1.size(); j++) {
      cout<<access(longest, i, j)<<" ";
    } 
    cout<<endl;
  }
#endif

  free(longest);
  free(dir);

  return 0;
}
