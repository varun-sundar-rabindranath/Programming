#ifndef __TRIE_H__
#define __TRIE_H__

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

#define ALPHABETS 26

/* Interface file for trie data structure */

/* Forward decl of trie structures */
typedef struct trie_node {
  trie_node * next_letter[ALPHABETS];
  int frequency;

  trie_node() {
    frequency = 0;
    for(int i = 0; i < ALPHABETS; i++) {
      next_letter[i] = NULL;
    }
  }
} trie_node;

typedef struct trie {
  trie_node *node;
  
  trie() {
    node = NULL;
  }
} trie;

/* Initialize the trie structure */
void init_trie(trie *t);

/* Free trie and trie_nodes associated with trie */
void destroy_trie(trie *t);

/* Add string str to trie */
void add_string(trie *t, string &str);

/* Print contents of the trie */
void print_trie(trie *t);

/* Allocate memory for a trie_node */
trie_node *create_trie_node();

/* Free node ptr */
void free_trie_node(trie_node *node);

/* Print trie_node's data; s is the string that led to that trie_node */
void print_trie_node_data(trie_node *node, string s);

/* Traverse trie and get frequencies of the strings */
vector<pair<string, int> > get_word_frequency(trie *t);

#endif
