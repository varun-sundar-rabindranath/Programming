#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int max_val = -1;
int min_val = INT_MAX;
void process_lis(vector<pair<int, int> > &llist, int element) {
  if(element < min_val) {
    pair<int, int> new_pair;
    new_pair.first = element;
    new_pair.second = 1;
    llist.push_back(new_pair);
  }

  if(element > max_val) {
  }
}

int lis(vector<int> &sequence) {
  vector<pair<int, int> > llist;

  for(int i = 0; i < sequence.size(); i++) {
    process_lis(llist, sequence[i]);
  }
}

int main() {

  int N;

  cin>>N;

  vector<int> sequence;

  for(int i = 0; i < N; i++) {
    int val;
    cin>>val;
    sequence.push_back(val);
  }

  cout<<lis(sequence)<<endl;

  return 0;
}
