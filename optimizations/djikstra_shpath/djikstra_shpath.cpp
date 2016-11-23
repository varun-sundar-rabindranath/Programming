#include <iostream>
using namespace std;

#include <cassert>
#include <stdio.h>
#include <stdlib.h>

#include "timer.h"

//#define USE_EDGE_LIST

#define access_graph(i, j) graph[i * V + j]

#define MAX_VAL 10000+1

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

/* Create adj graph */
int * create_adj_graph(int V, int E) {
  /* Allocate adjacency mat to realize graph */
  int *graph = NULL; 
  graph = (int *)calloc(V * V, sizeof(int));
  assert(graph != NULL && "Cannot calloc mem");

  /* Mark edges in graph */
  int num_edges = E;
  while(num_edges--) {
    int V1, V2, W;
    cin>>V1;
    cin>>V2;
    cin>>W;
    V1 -= 1;
    V2 -= 1;

    access_graph(V1, V2) = W;
    //access_graph(V2, V1) = W;
  }
  return graph;
} 

void shpath_adjmat(int *graph, int S, int D, int V, int E) {

  int *dist = (int *)calloc(V, sizeof(int));
  char *visited = (char *)calloc(V, sizeof(char));
  for(int v_iter = 0; v_iter < V; v_iter++) {
    dist[v_iter] = MAX_VAL;
  }
  dist[S] = 0;

  /* Find shortest path */
  for(int p_iter = 0; p_iter < V; p_iter++) {
    /* Find vertex with minimum distance from S */
    int current_vertex = -1;
    int min_val = MAX_VAL;
    for(int v_iter = 0; v_iter < V; v_iter++) {
      if(dist[v_iter] < min_val && !visited[v_iter]) {
	min_val = dist[v_iter];
	current_vertex = v_iter;
      }
    }

    if(current_vertex == -1) {
      /* No path between S and D */
      break;
    }

    if(p_iter == 0) { assert(current_vertex == S); }

    /* Mark current vertex as visited */
    visited[current_vertex] = 1;

    if(current_vertex == D) { break; }
    
    /* Update neighbours */
    for(int v_iter = 0; v_iter < V; v_iter++) {
      if(access_graph(current_vertex, v_iter) && !visited[v_iter]) {
	int new_dist = dist[current_vertex] + access_graph(current_vertex, v_iter);
	if(new_dist < dist[v_iter]) {
	  dist[v_iter] = new_dist;
	}
      }
    }
  }
  
  if(dist[D] == MAX_VAL) {
    /* No path */
    printf("NO\n");
  } else {
    printf("%d\n", dist[D]);
  }

  free(visited);
  free(dist);
}


void shpath_adjlist(list **adj_list, int S, int D, int V, int E) {
  int *dist = (int *)calloc(V, sizeof(int));
  char *visited = (char *)calloc(V, sizeof(char));
  for(int v_iter = 0; v_iter < V; v_iter++) {
    dist[v_iter] = MAX_VAL;
  }
  dist[S] = 0;

  /* Find shortest path */
  for(int p_iter = 0; p_iter < V; p_iter++) {
    /* Find vertex with minimum distance from S */
    int current_vertex = -1;
    int min_val = MAX_VAL;

    for(unsigned int v_iter = 0; v_iter + 3 < V; v_iter = v_iter + 4) {

      if(dist[v_iter + 0] < min_val && !visited[v_iter + 0]) {			
	min_val = dist[v_iter + 0];						
	current_vertex = v_iter + 0;						
      }					      
      if(dist[v_iter + 1] < min_val && !visited[v_iter + 1]) {			
	min_val = dist[v_iter + 1];						
	current_vertex = v_iter + 1;						
      }					      
      if(dist[v_iter + 2] < min_val && !visited[v_iter + 2]) {			
	min_val = dist[v_iter + 2];						
	current_vertex = v_iter + 2;						
      }					      
      if(dist[v_iter + 3] < min_val && !visited[v_iter + 3]) {			
	min_val = dist[v_iter + 3];						
	current_vertex = v_iter + 3;						
      }					      
    }

    /* loop for the left out v_iters */
    unsigned int left_out_viter_start = (V / 4) * 4;
    for(unsigned int left_out = left_out_viter_start; left_out < V; left_out++) {
      if(dist[left_out] < min_val && !visited[left_out]) {
	min_val = dist[left_out];
	current_vertex = left_out;
      }
    }


    if(current_vertex == -1) {
      /* No path between S and D */
      break;
    }

    if(p_iter == 0) { assert(current_vertex == S); }

    /* Mark current vertex as visited */
    visited[current_vertex] = 1;

    if(current_vertex == D) { break; }

    edge_list *e_list = adj_list[current_vertex]->e_list;
    while(e_list != NULL) {
      int to_vertex = e_list->e;
      if(!visited[to_vertex]) {
	int w = e_list->w;
	int new_dist = dist[current_vertex] + w;
	if(new_dist < dist[to_vertex]) {
	  dist[to_vertex] = new_dist;
	} 
      }
      e_list = e_list->nxt;
    }
  }
  
  if(dist[D] == MAX_VAL) {
    /* No path */
    printf("NO\n");
  } else {
    printf("%d\n", dist[D]);
  }

  free(visited);
  free(dist);
}

int main() {
  int N;
  
  // Get number of test cases
  cin>>N;

  while(N--) {
    int V, E;

    // Get num Vertices and Edges
    cin>>V;
    cin>>E;

    /* Creating graph representation */
#ifdef USE_EDGE_LIST
    list **adj_list = create_adj_list(V, E);
#else
    int *graph = create_adj_graph(V, E);
#endif
      
    int S, D;
    cin>>S;
    cin>>D;
    S -= 1;
    D -= 1;

    /* Execute algorithm */
#ifdef USE_EDGE_LIST
    startTimer();
    shpath_adjlist(adj_list, S, D, V, E);
    endTimer();
    fprintf(stderr, "djikstra adjlist exec time - %llu \n", getElapsedTime());
#else
    startTimer();
    shpath_adjmat(graph, S, D, V, E);
    endTimer();
    fprintf(stderr, "djikstra adjmat exec time - %llu \n", getElapsedTime());
#endif

    /* Free graph representation */
#ifdef USE_EDGE_LIST
    free_adjlist(adj_list, V);
#else 
    free(graph);
#endif
  }
  return 0;
}
