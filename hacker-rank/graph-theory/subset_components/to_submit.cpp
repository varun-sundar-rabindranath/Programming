#include <iostream> 
#include <vector>
#include <queue>
#include <cassert>
#include <cstdlib>
using namespace std;


//#include "adj_list.h"

typedef struct edge_list {
  int e;
  int w;
  struct edge_list *nxt;
}edge_list;

typedef struct list{
  int num_edges;
  edge_list *e_list;

  list() {
    num_edges = 0;
    e_list = NULL;
  }
}list;

//#include "edge_list_array.h"

/* There can be atmost V*V edge lists; Just create a pool of
 * edge list so we need not malloc/free it again and again
 */
edge_list **edge_list_pool;
int edge_list_used = 0;
int edge_list_total = 0;

void create_pool(int count) {
  edge_list_pool = (edge_list **)calloc(count, sizeof(void *));

  for(int i = 0; i < count; i++) {
    edge_list_pool[i] = (edge_list *)calloc(1, sizeof(edge_list));
  }

  edge_list_used = 0;
  edge_list_total = count;
}

edge_list *get_element() {
  edge_list * ptr = edge_list_pool[edge_list_used];
  edge_list_used++;
  assert(edge_list_used <= edge_list_total);
  return ptr;
}

void reset_edge_list_pool() {
  edge_list_used = 0;
}

void release_edge_list_pool() {
  for(int i = 0; i < edge_list_total; i++) {
    free(edge_list_pool[i]);
  }
  free(edge_list_pool);
  edge_list_total = 0;
  edge_list_used  = 0;
}


#define V 64
#define access(adj_mat, i, j) (adj_mat[i * V + j])

unsigned long long int sum = 0;

list **adj_list = NULL;
char *bfs_visited = NULL;
char *edge_recorder = NULL;

/* Check if the jth bit of edge desc is 1 */
inline bool is_bit_one(unsigned long long int edge_desc, int j) {
  unsigned long long int mask = 1;
  mask = mask << j;
  return (edge_desc & mask);
}

/* Adjacency list functions */
void add_edge_list(int V1, int V2) {
  edge_list *v1_to_v2 = get_element();
  v1_to_v2->e = V2;
  v1_to_v2->w = 1;
  v1_to_v2->nxt = NULL;

  edge_list *v2_to_v1 = get_element();
  v2_to_v1->e = V1;
  v2_to_v1->w = 1;
  v2_to_v1->nxt = NULL;

  v1_to_v2->nxt = adj_list[V1]->e_list;
  adj_list[V1]->e_list = v1_to_v2;

  v2_to_v1->nxt = adj_list[V2]->e_list;
  adj_list[V2]->e_list = v2_to_v1;
}

void create_adj_list(unsigned long long int *graph_edges, int num_edges) { 

  /*ditch our bfs visited */
  for(int i = 0; i < V; i++) {
    bfs_visited[i] = 0;
  }
  /* Reset edge recorder */
  for(int i = 0; i < V * V; i++) {
    edge_recorder[i] = 0;
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
	if(!access(edge_recorder, ones[ones_iter_i], ones[ones_iter_j])) {
	  add_edge_list(ones[ones_iter_i], ones[ones_iter_j]);
	  access(edge_recorder, ones[ones_iter_i], ones[ones_iter_j]) = 1;
	  access(edge_recorder, ones[ones_iter_j], ones[ones_iter_i]) = 1;
	}
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

    edge_list *el_iter = adj_list[current_node]->e_list;

    /* Get its children */
    while(el_iter != NULL) {
      int edge = el_iter->e; 
      if(!bfs_visited[edge]) {
	bfs_queue.push(edge);
	bfs_visited[edge] = 1;
      }

      el_iter = el_iter->nxt;
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

  create_adj_list(edge_desc, num_edges);

  count_comp();
  
  /* Reset adj list */
  reset_edge_list_pool();
  for(int i = 0; i < V; i++) {
    adj_list[i]->e_list = NULL;
  }
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

  create_pool(V * V);
  bfs_visited = (char *)calloc(V, sizeof(char));
  edge_recorder = (char *)calloc( V * V, sizeof(char));
  adj_list = (list **)calloc(V, sizeof(void *));
  /* Init adj list */
  for(int i = 0; i < V; i++) {
    adj_list[i] = (list *)calloc(1, sizeof(list));
  }

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

  /* Free adj list */
  for(int i = 0; i < V; i++) {
    free(adj_list[i]);
  }
  free(adj_list);
  free(bfs_visited);
  release_edge_list_pool();
  free(edge_recorder);

  return 0;
}

