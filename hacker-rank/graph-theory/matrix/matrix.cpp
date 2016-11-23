#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <climits>
#include <cstdlib>
using namespace std;

#define TOTAL_VERTICES 100000

#define DEBUG

typedef struct bfs_node {
  int id;
  vector<int> node_path;
  vector<int> weights;
}bfs_node;

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

void make_edge(list **adj_list, int v1, int v2, int w) {

  assert(v1 != v2);

  edge_list *el1 = (edge_list *)calloc(1, sizeof(edge_list));
  el1->e = v2;
  el1->w = w;
  el1->nxt = NULL;

  el1->nxt = adj_list[v1]->e_list;
  adj_list[v1]->e_list = el1;

  edge_list *el2 = (edge_list *)calloc(1, sizeof(edge_list));
  el2->e = v1;
  el2->w = w;
  el2->nxt = NULL;

  el2->nxt = adj_list[v2]->e_list;
  adj_list[v2]->e_list = el2;
}

list ** create_adjlist(vector<int> &s_vec, vector<int> &d_vec, vector<int> &w_vec, int n) {
  list ** adj_list = NULL;
  adj_list = (list **)calloc(n, sizeof(void *));
  assert(adj_list != NULL);

  for(int i = 0; i < n; i++) {
    adj_list[i] = (list *)calloc(1, sizeof(list));
  }

  assert(s_vec.size() == d_vec.size());
  assert(w_vec.size() == s_vec.size());

  for(int i = 0; i < s_vec.size(); i++) {
    if(s_vec[i] == d_vec[i]) { continue; }
    make_edge(adj_list, s_vec[i], d_vec[i], w_vec[i]);
  }

  return adj_list;
}

void free_adj_list(list **a_list, int n) {
  for(int i = 0; i < n; i++) {
    edge_list *e_iter = a_list[i]->e_list;

    while(e_iter != NULL) {
      edge_list *to_del = e_iter;
      e_iter = e_iter->nxt;
      free(to_del);
    }

    free(a_list[i]);
  }

  /* Free all pointers */
  free(a_list);
}

bfs_node * get_bfs_node (int id) {
  bfs_node * ret_val = NULL;
  ret_val = (bfs_node *)calloc(1, sizeof(bfs_node));
  assert(ret_val != NULL);
  ret_val->id = id;
  return ret_val;
}

bfs_node * append_bfs_node (int child, int weight, bfs_node *parent_node) {

  bfs_node *node = get_bfs_node(child);
  node->node_path = parent_node->node_path;
  node->weights = parent_node->weights;

  node->node_path.push_back(child); 
  node->weights.push_back(weight);
  return node;
}

int sever_connection(vector<int> &weights) {
  int min = INT_MAX;

  assert(weights.size() != 0);

  for(int i = 0; i < weights.size(); i++) {
    if(min > weights[i]) {
      min = weights[i];
    }
  } 

  assert(min != INT_MAX);
  return min;
}

int do_bfs(list **adj_list, int root, int n, bool *has_machine) {
  int min_weight = 0;

  bool *visited = NULL;
  visited = (bool *)calloc(n, sizeof(bool));
  assert(visited != NULL);

  /* Push root node */
  queue<bfs_node *> bfs_queue;

  /* Create bfs_node */
  bfs_queue.push(get_bfs_node(root));
  visited[root] = true;

  while(!bfs_queue.empty()) {
    bfs_node *curr_node = bfs_queue.front();
    bfs_queue.pop();

    int curr_id = curr_node->id;
    /* Iterate curr_node's children */
    edge_list *e_iter = adj_list[curr_id]->e_list;
    while(e_iter != NULL) {
      int child = e_iter->e;
      int weight = e_iter->w;
      if(!visited[child]) {

	bfs_node *node = NULL;
	if(has_machine[child]) {
	  vector<int> path_weights = curr_node->weights;
	  path_weights.push_back(weight);

#ifdef DEBUG
	  cout<<" Machine node - "<<child<<endl<<"  ";
	  for(int i = 0; i < path_weights.size(); i++) {
	    cout<<path_weights[i]<<"  ";
	  }
	  cout<<endl;
#endif

	  min_weight += sever_connection(path_weights);

	  node = get_bfs_node(child);
	} else {
	  node = append_bfs_node(child, weight, curr_node);
	}

	assert(node != NULL);

	bfs_queue.push(node);
	visited[child] = true;
      }

      e_iter = e_iter->nxt;
    }

    free(curr_node);
  }

  free(visited);
  return min_weight;
}

vector<vector<int> > find_neighbour(list **adj_list, int n, bool *has_machine, int root) {

  vector<vector<int> > ret_val;

  queue<int> bfs_queue;
  queue<vector<int> > bfs_weights;
  queue<vector<int> > bfs_path;

  /* Init variables */
  vector<int> w, p;
  p.push_back(root);

  bool * visited = NULL;
  visited = (bool *)calloc(n, sizeof(bool));

  bfs_queue.push(root);
  bfs_weights.push(w);
  bfs_path.push(p);
  visited[root] = true;

  while(!bfs_queue.empty()) {
    int curr_node = bfs_queue.front();
    w = bfs_weights.front();
    p = bfs_path.front();

    bfs_queue.pop();
    bfs_weights.pop();
    bfs_path.pop();

    /* Iterate curr_node's children */
    edge_list *e_iter = adj_list[curr_node]->e_list;
    while(e_iter != NULL) {
      int child = e_iter->e;
      int weight = e_iter->w;
      if(!visited[child]) {

	vector<int> new_weight = w;
	vector<int> new_path = p;

	new_weight.push_back(weight);
	new_path.push_back(child);

	if(has_machine[child]) {
	  ret_val.push_back(new_path);
	  ret_val.push_back(new_weight);
	}

	bfs_weights.push(new_weight);
	bfs_path.push(new_path);
	bfs_queue.push(child);

	visited[child] = true;
      }

      e_iter = e_iter->nxt;
    }
  }
  
  free(visited);
  return ret_val;
}

void find_neighbour_and_severe_connection(list **adj_list, int n, bool *has_machine, int root) {

  while(1) {
    /* Do bfs to find neighboour */
    find_neighbour(adj_list, has_machine, root);

  }
}

int solve(list **adj_list, int n, bool *has_machine, vector<int> &machine_nodes) {
  for(int i = 0; i < machine_nodes.size(); i++) {
    find_neighbour_and_severe_connection(adj_list, n, has_machine, machine_nodes[i]);
  }
}

int main() {

  int N, K;
  cin>>N;
  cin>>K;

  bool has_machine[TOTAL_VERTICES] = {false};

  for(int i = 0; i < TOTAL_VERTICES; i++) {
    has_machine[i] = false;
  }

  vector<int> s_vec, d_vec, w_vec;
  vector<int> machine_nodes;

  for(int i = 0; i < N - 1; i++) {
    int x, y, w;
    cin>>x;
    cin>>y;
    cin>>w;
    
    s_vec.push_back(x);
    d_vec.push_back(y);
    w_vec.push_back(w);
  }

  int root = 0; 
  for(int i = 0; i < K; i++) {
    int city;
    cin>>city;

    machine_nodes.push_back(city);
    has_machine[city] = true;

    if(i == 0) { root = city; }

  }

  list ** adj_list = create_adjlist(s_vec, d_vec, w_vec, N);

  int min_weight = do_bfs(adj_list, root, N, has_machine);

  cout<<min_weight;

  free_adj_list(adj_list, N);
  return 0;
}
