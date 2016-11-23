#include <iostream>
#include <cassert>
#include <vector>
#include <cstdio>

using namespace std;

#define MAX_N 1000007
#define BST_SIZE (MAX_N * 2 + 1)

int bst_arr[BST_SIZE];

void add_element(int *bst, int element) {
  int index = 1;

  while(bst[index] != 0) {
    if(element >= bst[index]) {
      index = 2 * index + 1;
    } else {
      index = 2 * index;
    }
    assert(index < BST_SIZE);
  }

  bst[index] = element;
}

void inorder(int *bst, int index) {

  assert(index < BST_SIZE);

  if(bst[index] != 0) {
    inorder(bst, 2 * index);
    fprintf(stderr, "%d ", bst[index]);
    inorder(bst, 2 * index + 1);
  }
}

int main() {

  for(int i = 0; i < BST_SIZE; i++) {
    bst_arr[i] = 0;
  } 

  for(int i = 1000; i >= 1; i--) {
    add_element(bst_arr, i);
    inorder(bst_arr, 1);
    cout<<endl;
    //cout<<"element "<<i<<" added "<<endl;
  }

  inorder(bst_arr, 1);
  return 0;
}

