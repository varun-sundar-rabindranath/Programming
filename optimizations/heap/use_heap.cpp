#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

#include "heap.h"
#include "timer.h"

#define MAX_ELEM 10000

bool check_sort(int *arr, int n) {
  for(int i = 1; i < n; i++) {
    if(arr[i - 1] > arr[i]) { return false; }
  }
  return true;
} 

int main() {
  int N = 0;
  cin>>N;

  int *sorted_array = NULL;
  sorted_array = (int *)calloc(N, sizeof(int));
  int *array = NULL;
  array = (int *)calloc(N, sizeof(int));

  /* Generate contents into array */
  for(int i = 0; i < N; i++) {
    cin>>array[i];
  }

  startTimer();
  /* Create heap */
  min_heap h(array, N);
  for(int i = 0; i < N; i++) {
    /* Get min */
    sorted_array[i] = h.extract_min();
  }
  endTimer();
  printf("Elapsed time - %llu \n", getElapsedTime());

  /* Check sorted */
  if(!check_sort(sorted_array, N)) { fprintf(stderr, "Sort fail \n"); }

  free(array);
  free(sorted_array);
}
