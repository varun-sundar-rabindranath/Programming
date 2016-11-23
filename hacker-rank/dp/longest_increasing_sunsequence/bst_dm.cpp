#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
using namespace std;

typedef struct bst_node {
  int element;
  int lis;
  bst_node *left;
  bst_node *right;

  bst_node() {
    left = NULL;
    right = NULL;
    element = 0;
    lis = 0;
  }
}bst_node;

bst_node *create_bst_node() {
  bst_node *bn = NULL;
  bn = (bst_node *)calloc(1, sizeof(bst_node));
  assert(bn != NULL);
  bn->left = NULL;
  bn->right = NULL;
  bn->element = 0;
  bn->lis = 0;
  return bn;
}

typedef struct bst {
  int num_elements;
  bst_node *root;

  bst() {
    num_elements = 0;
    root = NULL;
  }
}bst;

void bst_insert(bst *b, int element, int lis) {

  bst_node *bn = create_bst_node();
  bn->element = element;
  bn->lis = lis;

  if(b->root == NULL) {
    b->root = bn;
    return;
  }

  bst_node *p = b->root;
  bool element_added = false;
  while(!element_added) {
    assert(p != NULL);

    if(element >= p->element) {
      if(p->right == NULL) { 
	p->right = bn; 
	element_added = true;
      } else {
	p = p->right;
      }
    } else {
      if(p->left == NULL) {
	p->left = bn;
	element_added = true;
      } else {
	p = p->left;
      }
    }
  }
}

void inorder_rec(bst_node *bn) {
  if(bn != NULL) {
    inorder_rec(bn->left);
    fprintf(stderr, " %d ", bn->element);
    inorder_rec(bn->right);
  }
}

void inorder(bst *b) {
 inorder_rec(b->root); 
 cout<<endl;
}

int main() {

  bst b;

  for(int i = 1000; i > 0; i--) {
    bst_insert(&b, i, i * 10);
  }

  inorder(&b);

  return 0;
}
