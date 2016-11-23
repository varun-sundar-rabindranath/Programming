#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

int bin_search(vector<pair<int, int> > &arr, int element) {
  if(arr[0].first == element) { return arr[0].second; }
  if(arr[arr.size() - 1].first == element) { return arr[arr.size() - 1].second; }
  int left = 0;
  int right = arr.size() - 1;

  int index = (left + right) / 2;
  while(arr[index].first != element) {
    if(arr[index].first > element) {
      right = index;
    } else {
      left = index;
    }
    index = (right + left) / 2;
  }
  return arr[index].second;
}

int get_curr_position(vector<int> &seen_init_positions, int init_position) {

  if(seen_init_positions.size() == 0) { seen_init_positions.push_back(init_position); return 0;}
  if(seen_init_positions[seen_init_positions.size() - 1] < init_position) {
    seen_init_positions.push_back(init_position);
    return 0;
  }
  if(seen_init_positions[0] > init_position) {
    int ret_val = seen_init_positions.size();
    seen_init_positions.insert(seen_init_positions.begin(), init_position);
    return ret_val;
  }

  int left = 0;
  int right = seen_init_positions.size() - 1;

  int index = (left + right) / 2;
  int prev_index = -1;
  while(prev_index != index) {
    if(seen_init_positions[index] > init_position) {
      right = index;
    } else {
      left = index;
    }
    prev_index = index;
    index = (right + left) / 2;
  }

  int my_index = 0;
  if(init_position > seen_init_positions[index]) { 

    my_index = index + 1;
  } else {
    my_index = index;
  } 

  seen_init_positions.insert(seen_init_positions.begin() + my_index, init_position);

  return (seen_init_positions.size() - 1 - my_index);
}

int main() {
  int N;
  vector<int> begin_order;
  vector<pair<int, int> > begin_order_assoc;

  cin>>N;

  for(int i = 0; i < N; i++) {
    int val;
    cin>>val;
    begin_order.push_back(val);
    begin_order_assoc.push_back(pair<int, int>(val, i));
  }
  
#if 1
  sort(begin_order_assoc.begin(), begin_order_assoc.end());

  unsigned long long int cost = 0;
  vector<int> seen_init_positions;
  for(int i = 0; i < N; i++) {
    int id = begin_order_assoc[i].first;
    int position = begin_order_assoc[i].second;

    int curr_position = position + get_curr_position(seen_init_positions, position);
    
    assert(curr_position >= i);
    cost += curr_position - i; 
  }

  cout<<cost<<endl;
#endif

  return 0;
}
