#include<iostream>
using namespace std;

#include <time.h>
#include <cassert>

#include <stdio.h>
#include <stdlib.h>

#define V 10000
#define E 100000
#define WMAX 10000

/* Lower and upper bounds are incl. */
int get_rand(int lb, int ub) {

  assert(ub > lb);
  int r = rand();

  int diff = ub - lb;
  r = r % diff;
  return (lb + r);  
}

int main() {

  srand(time(NULL));

  cerr<<1<<endl;
  cerr<<V<<" "<<E<<endl;

  int num_edge_created = 0;
  /* Output E number of lines */  
  while (num_edge_created < E) {
    int v1, v2, w;
    v1 = get_rand(1, V);
    v2 = v1;
    while(v1 == v2) {
      v2 = get_rand(1, V);
    }
  
    w = get_rand(1, WMAX);

    cerr<<v1<<" "<<v2<<" "<<w<<endl;

    num_edge_created++;
  }

  cerr<<1<<" "<<9999<<endl;

  return 0;
}
