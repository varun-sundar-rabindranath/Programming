#include<iostream>
using namespace std;

#include <ctime>
#include <cstdlib>
#include <cstdio>

struct node {
    int value;
    struct node *left;
    struct node *right;
};

struct node *create_node(int value) {
    struct node * new_node = (struct node *)calloc(1, sizeof(struct node));
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

struct node * tree_insert(struct node *root, int value) {
    struct node *insert_node = create_node(value);
    if(root == NULL) {
	return insert_node;
    }

    struct node *traverse_node = root;
    while(traverse_node != NULL) {
	if(insert_node->value >= traverse_node->value) {
	    if(traverse_node->right != NULL) {
		traverse_node = traverse_node->right;
	    } else {
		traverse_node->right = insert_node;
		/* exit while */
		break;
	    }
	} else {
	    if(traverse_node->left != NULL) {
		traverse_node = traverse_node->left;
	    } else {
		traverse_node->left = insert_node;
		/* exit while */
		break;
	    }
	}
    }

   return root; 
}

void inorder_traversal(struct node *root) {
    struct node *p = root;
    if(p!=NULL) {
	inorder_traversal(p->left);
	printf("%d ", p->value);
	inorder_traversal(p->right);
    }
    return;
}

void morris_traversal(struct node *root) {
    struct node *current = root;
    
    while(current != NULL) {
	if(current->left == NULL) {
	    printf("%d ", current->value);
	    current = current->right;
	} else {
	    /* find inorder predecessor */
	    struct node *pred = current->left;
	    while(pred->right != NULL && pred->right != current) {
		pred = pred->right;
	    }

	    if(pred->right == NULL) {
		pred->right = current;
		current = current->left;
	    } else {
		printf("%d ", current->value);
		/* fix the tree */
		pred->right = NULL;
		current = current->right;
	    }
	}
    }
}

int main() {
    srand(time(NULL));

    struct node *root = NULL;

    for(int i = 0; i < 10; i++) {
	int value = rand() % 10;
	root = tree_insert(root, value);
    }

    inorder_traversal(root);

    /* Morris traversal */
    printf("\n");
    morris_traversal(root);

    return 0;
}
