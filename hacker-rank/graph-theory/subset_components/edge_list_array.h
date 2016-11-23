#ifndef __EDGE_LIST_ARRAY_H__

#include "adj_list.h"
#include <cstdlib>
using namespace std;

/* There can be atmost V*V edge lists; Just create a pool of
 * edge list so we need not malloc/free it again and again
 */
edge_list **edge_list_pool;
int *book_keeping;
int edge_list_used = 0;
int edge_list_total = 0;

void create_pool(int count) {
  edge_list_pool = (edge_list **)calloc(count, sizeof(void *));
  book_keeping = (int *)calloc(count, sizeof(int));

  for(int i = 0; i < count; i++) {
    edge_list_pool[i] = (edge_list *)calloc(1, sizeof(edge_list));
  }

  edge_list_used = 0;
  edge_list_total = count;
}

edge_list *get_element(int v1, int v2, int stride) {
  edge_list * ptr = edge_list_pool[v1 * stride + v2];
  edge_list_used++;
  assert(edge_list_used <= edge_list_total);
  assert(book_keeping[v1 * stride + v2] == 0);
  book_keeping[v1 * stride + v2]++;
  return ptr;
}

void reset_edge_list_pool() {
  /* Reset book keeping */
  for(int i = 0; i < edge_list_total; i++) {
    book_keeping[i] = 0;
  }
  edge_list_used = 0;
}

void give_element(int v1, int v2, int stride) {
  edge_list_used--;
  assert(book_keeping[v1 * stride + v2] == 1);
  book_keeping[v1 * stride + v2]--;
}

void release_edge_list_pool() {
  for(int i = 0; i < edge_list_total; i++) {
    free(edge_list_pool[i]);
  }
  free(edge_list_pool);
  free(book_keeping);
  edge_list_total = 0;
  edge_list_used  = 0;
}

#define __EDGE_LIST_ARRAY_H__
#endif
