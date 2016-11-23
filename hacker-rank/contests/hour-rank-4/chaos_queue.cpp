#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

int main() {
  int T;
  cin>>T;

  while(T--) {
    vector<int> final_queue;
    vector<int> init_queue;
    map<int ,int> final_positions;
    int n;

    cin>>n;

    for(int i = 0; i < n; i++) {
      int element;
      cin>>element;
      element--;

      final_queue.push_back(element);
      final_positions.insert(pair<int, int>(element, i));
      init_queue.push_back(i);
    }

    int bribe = 0;
    string ret_string = "";
    for(int i = n - 1; i >= 0; i--) {
      int fpos = final_positions[i];
      if(fpos < i) {
	if(i - fpos > 2) { ret_string = "Too chaotic"; break; }
	bribe += i - fpos;
      }
      
    }

    if(ret_string == "") { cout<<bribe<<endl; }
    else { cout<<ret_string<<endl; }

  }

  return 0;
}
