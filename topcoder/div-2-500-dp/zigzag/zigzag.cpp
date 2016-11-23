#include <iostream>
#include <vector>

using namespace std;

class ZigZag {
public :
  int longestZigZag(vector<int> sequence) {
    if(sequence.size() == 0) { return 0; }
    
    vector<int> longest;
    longest.push_back(1);

    int to_expect = 0;

    for(int i = 0; i < sequence.size() - 1; i++) {

      int prev_state_longest = longest[i];

      int what_we_have;
      if(sequence[i] - sequence[i + 1] == 0) { what_we_have = 0; }
      if(sequence[i] - sequence[i + 1] > 0) { what_we_have = 1; }
      if(sequence[i] - sequence[i + 1] < 0) { what_we_have = -1; }
      
      if(what_we_have == 0) {
	longest.push_back(prev_state_longest);
	continue;
      }

      if(to_expect == 0) {
	longest.push_back(prev_state_longest + 1);
	to_expect = what_we_have * -1;
	continue;
      }

      if(what_we_have == to_expect) {
	longest.push_back(prev_state_longest + 1);
	to_expect = what_we_have * -1;
      } else {
	longest.push_back(prev_state_longest);
      }
    }

    return longest[sequence.size() - 1];
  }
};

int main() {
  ZigZag z;

  int T;
  cin>>T;

  while(T--) {
    int K;
    cin>>K;

    vector<int> seq;
    for(int i = 0; i < K; i++) {
      int element;
      cin>>element;
      seq.push_back(element);
    }

    cout<<z.longestZigZag(seq)<<endl;
  }

  return 0;

}
