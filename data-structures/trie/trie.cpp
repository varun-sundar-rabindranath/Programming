
/* Trie implementation */

#include "trie.hpp"

/* Initialize the trie structure */
void init_trie(trie *t) {
  t->node = create_trie_node();
}


/* Add string str to trie */
void add_string(trie *t, string &str) {
  trie_node *node = t->node;

  for(int i = 0; i < str.size(); i++) {
    char c = str[i];
    int alpha = c - 97;
    if(node->next_letter[alpha] == NULL) {
      node->next_letter[alpha] = create_trie_node(); 
    } 
    node = node->next_letter[alpha];
  }
  node->frequency++;
}  

void print_trie_recursively(trie_node *node, string s) {
  if(node->frequency != 0) {
    print_trie_node_data(node, s);
  }
  for(int i = 0; i < ALPHABETS; i++) {
    if(node->next_letter[i] != NULL) {
      char c[2];
      c[0] = i + 97;
      c[1] = '\0';
      print_trie_recursively(node->next_letter[i], s + string(c));
    }
  }
}

/* Print contents of the trie */
void print_trie(trie *t) {
  print_trie_recursively(t->node, "");
}

/* Print trie_node's data; s is the string that led to that node */
void print_trie_node_data(trie_node *node, string s) {
  cout<<s<<" --> "<<node->frequency<<endl;
}

/* Allocate memory for a trie_node */
trie_node *create_trie_node() {
  trie_node *node;
  node = (trie_node *)calloc(1, sizeof(trie_node));
  return node;
}

/* Free node ptr */
void free_trie_node(trie_node *node) {
  free(node);
}

void destroy_trie_recursively(trie_node *node) {
  for(int i = 0; i < ALPHABETS; i++) {
    if(node->next_letter[i] != NULL) {
      destroy_trie_recursively(node->next_letter[i]);
    }
  }
  free_trie_node(node);
}

/* Free trie and trie_nodes associated with trie */
void destroy_trie(trie *t) {
  /* Destroy all the trie nodes associated */
  destroy_trie_recursively(t->node);
}

void get_word_frequency_recursively(trie_node *node, string s, vector<pair<string, int> > &vec) {
  if(node->frequency != 0) {
    vec.push_back(pair<string, int> (s, node->frequency));
  }
  for(int i = 0; i < ALPHABETS; i++) {
    if(node->next_letter[i] != NULL) {
      char c[2];
      c[0] = i + 97;
      c[1] = '\0';
      get_word_frequency_recursively(node->next_letter[i], s + string(c), vec);
    }
  }
}

/* Traverse trie and get frequencies of the strings */
vector<pair<string, int> > get_word_frequency(trie *t) {
  vector<pair<string, int> > ret_vector;
  get_word_frequency_recursively(t->node, "", ret_vector);
  return ret_vector;
}
