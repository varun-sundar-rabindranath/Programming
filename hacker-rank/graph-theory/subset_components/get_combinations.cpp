#include <iostream>
#include <cassert>
using namespace std;

#include <vector>

#include <stdio.h>
#include <stdlib.h>

#define ELEMENTS 20

int comb_count = 0;

int factorial(int n) {
  if(n == 0) { return 1; }
  int fac = 1;
  for(int i = 1; i <= n; i++) {
    fac *= i;
  }
  return fac;
}
int ncr(int n, int r) {
  if(n < r) {
    cout<<" n < r ; exiting ";
    exit(0);
  }
  int n_fac, n_minus_r_fac, r_fac;
  n_fac = factorial(n);
  r_fac = factorial(r);
  n_minus_r_fac = factorial(n - r);

  return ((n_fac)/(n_minus_r_fac * r_fac));
}

void print_combination(vector<int> comb) {
  for(int i = 0; i < comb.size(); i++) {
    cout<<comb[i]<<" ";
  }
  cout<<endl;
}
void add_one(int *curr_comb, int curr_comb_count, int arr[ELEMENTS], int index, int num_elements) {
  for(int i = index; i < num_elements; i++) {
    /* Add stuff to current_combination */
    curr_comb[curr_comb_count] = arr[i];
    curr_comb_count++;

    /* Print combination */
    comb_count++;

    add_one(curr_comb, curr_comb_count, arr, i + 1, num_elements);

    /* Remove stuff from current_combination */
    curr_comb_count--;
  }
}

void get_comb(int arr[ELEMENTS], int num_elements) {
  int *calc_vector = NULL;
  calc_vector = (int *)calloc(ELEMENTS, sizeof(int));
  assert(calc_vector != NULL); 

  add_one(calc_vector, 0, arr, 0, ELEMENTS);

  free(calc_vector);
}

int calc_comb_count(int n) {
  int ret = 0;
  for(int i = 0; i <= n; i++) {
    ret += ncr(n, i);
  }
  return ret;
}

int main() {
  int arr[ELEMENTS];
  for(int i = 0; i < ELEMENTS; i++) {
    arr[i] = i;
  }

  /* calc_comb_count */
  //int calculated_comb_count = calc_comb_count(ELEMENTS);

  get_comb(arr, ELEMENTS);

  /* Add zero subset */
  comb_count++;

  //cout<<"Calculated comb count "<<calculated_comb_count;
  cout<<"; Comb count "<<comb_count<<endl;
  return 0;
}
