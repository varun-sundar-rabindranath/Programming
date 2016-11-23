#ifndef __ADJ_LIST_H__
#define __ADJ_LIST_H__

#include <iostream>
#include <cstdlib>
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

#if 0
void add_to_edge_list(int e, int w, list *l) {
  if(l->e_list == NULL) {
    l->e_list = (edge_list *)calloc(1, sizeof(edge_list));
    assert(l->e_list != NULL);
    l->e_list->e = e;
    l->e_list->w = w;
    l->e_list->nxt = NULL;
  } else {
    edge_list *node = NULL;
    node = (edge_list *)calloc(1, sizeof(edge_list));
    assert(node != NULL);
    node->e = e;
    node->w = w;
    node->nxt = l->e_list;
    l->e_list = node;
  }
  l->num_edges++;
  return;
}

void free_list(list *l) {
  edge_list *e_list = l->e_list;
  
  while(e_list) {
    edge_list *to_del = e_list;
    e_list = e_list->nxt;
    free(to_del);
  }
  free(l);
}

void free_adjlist(list **adj_list, int V) {
  for(int i = 0; i < V; i++) {
    free_list(adj_list[i]);
  }
  free(adj_list);
}

/* Create adj list */
list** create_adj_list(int V, int E) {
  list **adj_list = (list **)calloc(V, sizeof(list *)); 

  /* Initiliasze all lists */
  for(int i = 0; i < V; i++) {
    adj_list[i] = (list *)calloc(1, sizeof(list));
  }

  int num_edges_created = 0;

  while(num_edges_created < E) {
    int v1, v2, w;
    cin>>v1;
    cin>>v2;
    cin>>w;

    v1 -= 1;
    v2 -= 1;

    add_to_edge_list(v2, w, adj_list[v1]);
    //add_to_edge_list(v1, w, adj_list[v2]);

    num_edges_created++;
  }

  return adj_list;
}
#endif

#endif // __ADJ_LIST_H__
