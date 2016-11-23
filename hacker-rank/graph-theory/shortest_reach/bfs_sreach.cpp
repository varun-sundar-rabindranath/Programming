#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <cassert>
using namespace std;

#define W 6

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

void make_edge(list **adj_list, int v1, int v2) {

  assert(v1 != v2);

  edge_list *el1 = (edge_list *)calloc(1, sizeof(edge_list));
  el1->e = v2;
  el1->w = W;
  el1->nxt = NULL;

  el1->nxt = adj_list[v1]->e_list;
  adj_list[v1]->e_list = el1;

  edge_list *el2 = (edge_list *)calloc(1, sizeof(edge_list));
  el2->e = v1;
  el2->w = W;
  el2->nxt = NULL;

  el2->nxt = adj_list[v2]->e_list;
  adj_list[v2]->e_list = el2;
}

list ** create_adjlist(vector<int> &s_vec, vector<int> &d_vec, int n) {
  list ** adj_list = NULL;
  adj_list = (list **)calloc(n, sizeof(void *));
  assert(adj_list != NULL);

  for(int i = 0; i < n; i++) {
    adj_list[i] = (list *)calloc(1, sizeof(list));
  }

  assert(s_vec.size() == d_vec.size());

  for(int i = 0; i < s_vec.size(); i++) {
    if(s_vec[i] == d_vec[i]) { continue; }
    make_edge(adj_list, s_vec[i], d_vec[i]);
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

vector<int> get_spath(list **adj_list, int s, int n) {
  vector<bool> bfs_visited;
  vector<int> bfs_depth;
  for(int i = 0; i < n; i++) {
    bfs_visited.push_back(false);
    bfs_depth.push_back(-1);
  }

  
  queue<int> bfs_queue;
  bfs_queue.push(s);
  bfs_visited[s] = true;
  bfs_depth[s] = 0;


  while(!bfs_queue.empty()) {
    int curr_node = bfs_queue.front();
    bfs_queue.pop();


    /* Iterate curr_node's children */
    edge_list *e_iter = adj_list[curr_node]->e_list;
    while(e_iter != NULL) {
      int child = e_iter->e;
      if(!bfs_visited[child]) {
	bfs_queue.push(child);
	bfs_visited[child] = true;
	bfs_depth[child] = bfs_depth[curr_node] + 1;
      }
      e_iter = e_iter->nxt;
    }
  } 

  return bfs_depth;
}

int main() {
  int T;
  int N;
  int M;
  int S;

  cin>>T;

  while(T) {
    cin>>N;
    cin>>M;

    vector<int> s_vec, d_vec;

    for(int i = 0; i < M; i++) {
      int s, d;
      cin>>s;
      cin>>d;

      s = s - 1;
      d = d - 1;

      s_vec.push_back(s);
      d_vec.push_back(d);
    }

    list **adj_list = create_adjlist(s_vec, d_vec, N);

    cin>>S;
    S = S - 1;

    vector<int> depth = get_spath(adj_list, S, N);

    for(int i = 0; i < depth.size(); i++) {
      if(i == S) { continue; }
      if(depth[i] == -1) {
	cout<<"-1 ";
      } else {
	cout<<depth[i] * W<<" ";
      }
    }

    cout<<endl;

    free_adj_list(adj_list, N);

    T--;
  }
  return 0;
}
