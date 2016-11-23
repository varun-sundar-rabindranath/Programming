#include <iostream>
#include <stdio.h>
#include <queue>
using namespace std;

int main() {
  queue<int> q;

  for(int i = 0; i < 10; i++) {
    q.push(i);
  }

  while(!q.empty()) {
    fprintf(stderr, " %d \n", q.front());
    q.pop();
  }
  return 0;
}
