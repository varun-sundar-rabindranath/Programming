#include <iostream> 
#include <vector>
#include <queue>
#include <cassert>
#include <cstdlib>
using namespace std;

#define V 64

unsigned long long int sum = 0;

int bit_count(unsigned long long int n) {
  int count = 0;
  while(n) {
    n = n & (n - 1);
    count++;
  }
  return count;
}

bool inline can_merge(unsigned long long int c1, unsigned long long int c2) {
  return c1 & c2;
}

void count_for_this_comb(unsigned long long int *edge_desc, int num_edges) {
  
  vector<unsigned long long int> components;

  for(int i = 0; i < num_edges; i++) {
    vector<unsigned long long int> merge_components;
    unsigned long long int desc = edge_desc[i];

    if(desc == 0) { continue; }

    vector<unsigned long long int>::iterator component_iter;

    for(component_iter = components.begin(); component_iter != components.end(); component_iter++) {
      if(can_merge(*component_iter, desc)) {
	merge_components.push_back(*component_iter);
	component_iter = components.erase(component_iter);
	component_iter--;
      }
    }
    
    if(merge_components.size() == 0) {
      components.push_back(desc);
    } else {
      unsigned long long int new_comp = desc;
      for(int k = 0; k < merge_components.size(); k++) {
	new_comp |= merge_components[k];
      }
      components.push_back(new_comp);
    }
  }

  /* Count num conn components */
  unsigned long long int part_of_some_component = 0;
  for(int i = 0; i < components.size(); i++) {
    part_of_some_component |= components[i];
  }

  int num_trivial_components = V - bit_count(part_of_some_component);

  sum += num_trivial_components + components.size();

//#define DEBUG
#ifdef DEBUG
  cout<<" Processed edges ... ";
  for(int i = 0; i < num_edges; i++) {
    cout<<edge_desc[i]<<" ";
  }
  cout<<" | "<< num_trivial_components + components.size();
  cout<<endl;
#endif

}


unsigned long long int num_comb = 0;

void add_one(unsigned long long int* curr_comb,
	     int curr_comb_count,  
	     vector< unsigned long long int> arr, 
	     int index, 
	     int num_elements) {

  for(int i = index; i < num_elements; i++) {

    /* Add stuff to current_combination */
    curr_comb[curr_comb_count] = arr[i];
    curr_comb_count++;

    num_comb++;
    /* Print combination */
    count_for_this_comb(curr_comb, curr_comb_count);

    add_one(curr_comb, curr_comb_count, arr, i + 1, num_elements);

    /* Remove stuff from current_combination */
    curr_comb_count--;
  }

}

void count_conn_comp(vector<unsigned long long int> edge_desc) {
  unsigned long long int *comb_vector = NULL;
  comb_vector = (unsigned long long int *)calloc(20, sizeof(unsigned long long int));
  assert(comb_vector != NULL);

  add_one(comb_vector, 0, edge_desc, 0, edge_desc.size());

  free(comb_vector);
}

int main() {

  int num_edges = 0;
  vector<unsigned long long int> edge_desc;

  cin>>num_edges;
  
  for(int i = 0; i < num_edges; i++) {
    unsigned long long int desc;
    cin>>desc;
    edge_desc.push_back(desc);
  }

  count_conn_comp(edge_desc);

  /* For null set */
  sum += 64;

  cout<<sum;
  return 0;
}

