#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;

void fill_vector(int a, int b, vector<int>* filled_vector=NULL) {
	if (filled_vector == NULL) {
		return;
	} else {
		(*filled_vector).push_back(a);
		(*filled_vector).push_back(b);
	}
}


int main() {
	vector<int> filled_vector;
	fill_vector(1, 2);
	fill_vector(1, 2, &filled_vector);
	fprintf(stderr, "Filled vector sze - %lu \n", filled_vector.size());
	
	return 0;
}
