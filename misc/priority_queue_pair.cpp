#include <iostream>
#include <queue>
using namespace std;

typedef struct st {
  int a; int b; int c;
  st() {
    a = b = c = 0;
  }
  st(int a_, int b_, int c_) {
    a = a_;
    b = b_;
    c = c_;
  }
}st;

#if 1
typedef struct comp {
    bool operator()( const pair<int, st>& p1, const pair<int, st>& p2 ) const 
    {  
	    return p1.first > p2.first;
#if 0
	    if( p1.first < p2.first ) return true;
	    if( p2.first < p1.first ) return false;
	    return p1.second < p2.second;
#endif
    }
}comp;
#endif

int main() {
  priority_queue<pair<int, st>, vector<pair<int, st> >, comp> st_queue;
  st_queue.push(pair<int, st>(10, st(1, 2, 3)));
  st_queue.push(pair<int, st>(8, st(1, 5, 3)));
  st_queue.push(pair<int, st>(9, st(1, 6, 3)));
  st_queue.push(pair<int, st>(11, st(1, 7, 3)));
  

  while(!st_queue.empty()) {
    cout<<st_queue.top().first<<endl;
    st_queue.pop();
  }
  return 0;
}
