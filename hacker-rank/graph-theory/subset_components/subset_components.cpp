#include <iostream> 
#include <vector>
#include <queue>
#include <cassert>
#include <cstdlib>
using namespace std;

#define V 64
#define access(adj_mat, i, j) (adj_mat[i * V + j])

unsigned long long int sum = 0;

char *adj_mat = NULL;
char *bfs_visited = NULL;

/* Check if the jth bit of edge desc is 1 */
inline bool is_bit_one(unsigned long long int edge_desc, int j) {
  unsigned long long int mask = 1;
  mask = mask << j;
  return (edge_desc & mask);
}

inline void mark_edge(char *adj_mat, int v1, int v2) {
  if(v1 == v2) { return; }
  if(v1 < v2) {
    access(adj_mat, v1, v2) = 1;
  }
  if(v2 < v1) {
    access(adj_mat, v2, v1) = 1;
  }
}

inline bool has_edge(char *adj_mat, int v1, int v2) {
  if(v1 == v2) { return false; }
  if(v1 < v2) {
    return access(adj_mat, v1, v2);
  } else {
    return access(adj_mat, v2, v1);
  }
}

void create_graph(unsigned long long int * graph_edges, int num_edges) {

  /* zero adj_mat and bfs_visited */
  for(int i = 0; i < V * V; i++) {
    adj_mat[i] = 0;
  }
  for(int i = 0; i < V; i++) {
    bfs_visited[i] = 0;
  }

  for(int edge_desc_iter = 0; edge_desc_iter < num_edges; edge_desc_iter++) {
    unsigned long long int edge_desc = graph_edges[edge_desc_iter];
    vector<int> ones;
    for(int j = 0; j < 64; j++) {
      /* Record where ever we have one */
      if(is_bit_one(edge_desc, j)) {
	ones.push_back(j);
      }
    }

    /* Connect all vertices that have been marked with 1 */
    for(int ones_iter_i = 0; ones_iter_i < ones.size(); ones_iter_i++) {
      for(int ones_iter_j = ones_iter_i; ones_iter_j < ones.size(); ones_iter_j++) {
	mark_edge(adj_mat, ones[ones_iter_i], ones[ones_iter_j]);
      }
    }
  } 
}

void do_bfs(int root) {
  queue<int> bfs_queue;
  bfs_queue.push(root);
  bfs_visited[root] = 1;

  while(!bfs_queue.empty()) {
    int current_node = bfs_queue.front();
    bfs_queue.pop();

    /* Get its children */
    for(int i = 0; i < V; i++) {
      if(has_edge(adj_mat, current_node, i) &&
	 !bfs_visited[i]) {
	bfs_queue.push(i);
	bfs_visited[i] = 1;
      }
    }

  }
} 

void count_comp() {
  for(int i = 0; i < V; i++) {
    if(!bfs_visited[i]) {
      sum++;
      do_bfs(i);
    }
  }
}

void count_for_this_comb(unsigned long long int *edge_desc, int num_edges) {

  create_graph(edge_desc, num_edges);
  count_comp();

}

void add_one(unsigned long long int* curr_comb,
	     int curr_comb_count,  
	     vector< unsigned long long int> arr, 
	     int index, 
	     int num_elements) {

  for(int i = index; i < num_elements; i++) {

    /* Add stuff to current_combination */
    curr_comb[curr_comb_count] = arr[i];
    curr_comb_count++;

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

  adj_mat = (char *)calloc(V * V, sizeof(char));
  bfs_visited = (char *)calloc(V, sizeof(char));
  assert(adj_mat != NULL);
  assert(bfs_visited != NULL);

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

  free(adj_mat);
  free(bfs_visited);

  return 0;
}

