#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

#define NUM_NODES 100001

#define INTERVAL 1000
#define INTERVAL_2 100

int get_ancestor2(int *parent, int node, int k) {

  if(k == 0) { return node; }
  while(k-- && (node != -1)) {
    node = parent[node];
  }
  return node;
}

int get_ancestor(int *parent, int *interval2, int node, int k) {
  if(k == 0) { return node; }
  int ancestor = node;
  while(k >= INTERVAL_2 && (ancestor != -1)) {
    ancestor = interval2[ancestor];
    k = k - INTERVAL_2;
  }

  ancestor = get_ancestor2(parent, ancestor, k);
  return ancestor;
}

int main() {
  int parent[NUM_NODES];
  int interval_ancestor[NUM_NODES];
  int interval2_ancestor[NUM_NODES];

  int T;
  cin>>T;

  while(T--) {
    /* Init all values to -1 */
    for(int i = 0; i < NUM_NODES; i++) {
      parent[i] = -1;
      interval_ancestor[i] = -1;
      interval2_ancestor[i] = -1;
    } 

    int n;
    cin>>n;
    
    vector<int> children;

    for(int i = 0; i < n; i++) {
      int c, p;
      cin>>c;
      cin>>p;

      parent[c] = p;
      children.push_back(c);
    }

    /* Update interval acestor */
    for(int i = 0; i < children.size(); i++) {
      int child = children[i];
      interval2_ancestor[child] = get_ancestor2(parent, child, INTERVAL_2);
    }

    for(int i = 0; i < children.size(); i++) {
      int child = children[i];
      interval_ancestor[child] = get_ancestor(parent, interval2_ancestor, child, INTERVAL);
    }

    int q;
    cin>>q;

    for(int i = 0; i < q; i++) {
      int mode;
      cin>>mode;
      switch(mode) {
	case 0 :
		  {
		    int c, p;
		    cin>>p;
		    cin>>c;
		    parent[c] = p;
		    interval2_ancestor[c] = get_ancestor2(parent, c, INTERVAL_2);
		    interval_ancestor[c] = get_ancestor(parent, interval2_ancestor, c, INTERVAL);
		  }
		  break;
	case 1 : 
		  {
		    int c;
		    cin>>c;
		    parent[c] = -1;
		    interval_ancestor[c] = -1;
		  }
		  break;
	case 2 :
		  {
		    int node, k;
		    cin>>node;
		    cin>>k;
		    
		    int ancestor = node;
		    while(k >= INTERVAL && (ancestor != -1)) {
		      ancestor = interval_ancestor[ancestor];
		      k = k - INTERVAL;
		    }

		    ancestor = get_ancestor(parent, interval2_ancestor, ancestor, k);

		    if(ancestor == -1) {
		      cout<<"0"<<endl;
		    } else {
		      cout<<ancestor<<endl;
		    }

		  }
		  break;
	default :
		  assert(0);
      }
    }
  }
  return 0;
}
