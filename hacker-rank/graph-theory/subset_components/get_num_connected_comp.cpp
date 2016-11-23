#include <iostream>
using namespace std;

#include <vector>
#include <queue>

#include <stdlib.h>

#define V 64

#define access(adj_mat, i, j) (adj_mat[i * V + j])

/* There can be atmost V*V edge lists; Just create a pool of
 * edge list so we need not malloc/free it again and again
 */

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

void create_graph(char *adj_mat, vector<unsigned long long int> graph_edges) {
  for(int edge_desc_iter = 0; edge_desc_iter < graph_edges.size(); edge_desc_iter++) {
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

void do_bfs(char *adj_mat, char *bfs_visited, int root) {
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

int count_connected_components(char *adj_mat) {
  char *bfs_visited = NULL;
  bfs_visited = (char *)calloc(V, sizeof(char));
  int conn_comp = 0;
  for(int i = 0; i < V; i++) {
    if(!bfs_visited[i]) {
      do_bfs(adj_mat, bfs_visited, i);
      conn_comp++;
    }
  }
  free(bfs_visited);
  return conn_comp;
}

int main() {
  char *adj_mat = NULL;
  adj_mat = (char *)calloc(V * V, sizeof(char));

  vector<unsigned long long int> graph_edges;
 // graph_edges.push_back(2);
  graph_edges.push_back(5);
  graph_edges.push_back(9);
  

  create_graph(adj_mat, graph_edges);

  int conn_comp = count_connected_components(adj_mat);

  cout<<"Number of connected components - "<<conn_comp<<endl;

  free(adj_mat);
}
