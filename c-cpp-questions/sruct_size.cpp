#include <iostream>
#include <stdio.h>
using namespace std;

typedef struct element {
	int a;
	char b;
}element;

int main() {
	element e1;
	fprintf(stderr, "size of e1 %lu \n", sizeof(e1));
	element e_arr[10];
	fprintf(stderr, "size of earr %lu \n", sizeof(e_arr));
	return 0;
}
