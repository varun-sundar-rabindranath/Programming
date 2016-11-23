#include <iostream>
#include <cassert>
#include <climits>
using namespace std;

#include "heap.h"

/* Pass indices and get back indices */
#define LEFT(i) (2*i + 1)
#define RIGHT(i) (2*i + 2)
#define PARENT(i) ((i - 1) / 2)

#define min(x, y) (((x ^ y) & (-(x < y))) ^ y)

/* Run min heapify at idx */
void min_heap::min_heapify(int idx) {
  int min_index = idx;
  int left = LEFT(idx);
  int right = RIGHT(idx);

#if 0
  int max = INT_MAX;
  int v1 = INT_MAX, v2 = INT_MAX, v3 = INT_MAX;
  v1 = array[idx];
  if(right < heap_size) {
    v2 = array[left];
    v3 = array[right];
  } else {
    if( left < heap_size) {
      v2 = array[left];
    }
  }

  int min_val = min(v1, v2);
  min_val = min(min_val, v3);

  if(v1 == min_val) { 
    min_index = idx; 
  } else if(v2 == min_val) {
    min_index = left;
  } else if(v3 == min_val) {
    min_index = right;
  }
#endif

  if(left < heap_size && array[left] < array[idx]) {
    min_index = left;
  }
  if(right < heap_size && array[right] < array[min_index]) {
    min_index = right;
  }

  if(min_index != idx) {
    /* Swap idx and min_index contents */
    int tmp = array[idx];
    array[idx] = array[min_index];
    array[min_index] = tmp;

    min_heapify(min_index);
  }
}

/* Build a min_heap */
void min_heap::build_min_heap() {
  assert(array != NULL);

  /* All elements should be heapified !! */
  heap_size = array_length;
  
  /* Get the first last parent in the tree */
  int parent = PARENT(array_length - 1);

  while(parent >= 0) {
    min_heapify(parent);
    parent--;
  }

}

/* Extract the root of the heap */
int min_heap::extract_min() {
  if(heap_size == 0) { return -1; }
  int ret_val = array[0]; 

  /* Swap a[0] with a[heap_size - 1] */
  int tmp = array[0];
  array[0] = array[heap_size - 1];
  array[heap_size - 1] = tmp;

  heap_size--;

  min_heapify(0);
  return ret_val;
}
