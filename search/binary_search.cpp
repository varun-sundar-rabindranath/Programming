#include <iostream>
#include <vector>
using namespace std;

int bin_search(vector<int> arr, int val) {

  // Do binary search
  int s = 0;
  int e = arr.size();
  int idx = (s + e) / 2;
  int prev_idx = -1;

  while (prev_idx != idx) {
    
    prev_idx = idx;
      
    if (arr[idx] == val) {
          break;
    }
    if (arr[idx] < val) {
        s = idx;
    }
    if (arr[idx] > val) {
        e = idx;
    }
  
    idx = (s + e) / 2;
  }

  int diff = val - arr[idx];
  assert (diff >= 0);
  
  if (diff == 0) { return idx; }

  if (idx + 1 < e) {
    if (diff < (val - arr[idx + 1]))
  }

  return idx;
}

int main() {

  vector<int> vec;

  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(5);
  vec.push_back(6);
  vec.push_back(7);
  vec.push_back(10);
  vec.push_back(12);
  vec.push_back(13);
  vec.push_back(14);
  vec.push_back(17);
  vec.push_back(21);

  //for (int i = 0; i <= 21; i++) {
    cout<<" 20 is at idx - "<<bin_search(vec, 20)<<endl;
  //}

  return 0;

}
