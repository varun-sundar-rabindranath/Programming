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

#endif // __ADJ_LIST_H__
