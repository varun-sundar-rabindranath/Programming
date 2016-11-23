#include <iostream>
#include <cstdlib>
#include <cassert>
#include <climits>
using namespace std;

int main() {

  unsigned long long int int_max = INT_MAX;

  int N, E;

  cin>>N;
  cin>>E;

  /* Alloc adj mat */
  int *adj_mat = NULL;
  adj_mat = (int *)calloc(N * N, sizeof(int));
  assert(adj_mat != NULL);

  /* Init adj mat */
  for(int i = 0; i < N * N; i++) {
    adj_mat[i] = int_max;
  }

  for(int i = 0; i < E; i++) {
    int s, d, w;
    cin>>s;
    cin>>d;
    cin>>w;

    s = s - 1;
    d = d - 1;

    adj_mat[s * N + d] = w;
  }

  /* Do floyd-warshall */
  for(int k = 0; k < N; k++) {
    for(int i = 0; i < N; i++) {
      for(int j = 0; j < N; j++) {
	if(adj_mat[i * N + k] == int_max || adj_mat[k * N + j] == int_max) {
	  /* do nothing */
	} else{ 
	  adj_mat[i * N + j] = min(adj_mat[i * N + j], adj_mat[i * N + k] + adj_mat[k * N + j]);
	}
      }
    }
  }

  int Q;
  cin>>Q;
  for(int i = 0; i < Q; i++) {
    int s, d;
    cin>>s;
    cin>>d;
    s = s - 1;
    d = d - 1;

    if(s == d) {
      cout<<"0"<<endl;
    } else if(adj_mat[s * N + d] == int_max) {
      cout<<"-1"<<endl;
    } else {
      cout<<adj_mat[s * N + d]<<endl;
    }
  }

  free(adj_mat);
  return 0;
}
