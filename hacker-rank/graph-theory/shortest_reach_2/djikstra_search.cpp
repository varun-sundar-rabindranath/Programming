#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <climits>
using namespace std;

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

void get_spath(list **adj_list, vector<int> &dist, int s, int n) {
  vector<bool> visited;
  for(int i = 0; i < n; i++) {
    visited.push_back(false);
    dist.push_back(INT_MAX);
  }

  dist[s] = 0;

  for(int iter = 0; iter < n; iter++) {
    int curr_node;
    int min_val = INT_MAX;

    for(int i = 0; i < n; i++) {
      if(dist[i] < min_val && !visited[i]) {
	min_val = dist[i];
	curr_node = i;
      }
    }

    if(min_val == INT_MAX) { break; }

    /* Iterate through curr_node's children */
    edge_list *e_iter = adj_list[curr_node]->e_list;
    while(e_iter != NULL) {
      int child = e_iter->e;
      if(!visited[child]) {
	int weight_to_child = e_iter->w;
      	if(dist[child] > dist[curr_node] + weight_to_child) {
      	  dist[child] = dist[curr_node] + weight_to_child;
      	}
      }
      e_iter = e_iter->nxt;
    }

    visited[curr_node] = true;
  }
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

    vector<int> s_vec, d_vec, w_vec;

    for(int i = 0; i < M; i++) {
      int s, d, w;
      cin>>s;
      cin>>d;
      cin>>w;

      s = s - 1;
      d = d - 1;

      s_vec.push_back(s);
      d_vec.push_back(d);
      w_vec.push_back(w);
    }

    list **adj_list = create_adjlist(s_vec, d_vec, w_vec, N);

    cin>>S;
    S = S - 1;

    vector<int> dist;
    get_spath(adj_list, dist, S, N);

    for(int i = 0; i < dist.size(); i++) {
      if(i == S) { continue; }
      if(dist[i] == INT_MAX) {
	cout<<"-1 ";
      } else {
	cout<<dist[i]<<" ";
      }
    }

    cout<<endl;

    free_adj_list(adj_list, N);

    T--;
  }
  return 0;
}
