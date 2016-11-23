#include <iostream>
using namespace std;
#include <stdio.h>

int calc_sum(int orig_table[7][7], int i_end, int j_end) {
	int sum = 0;
	for(int i = 0; i <= i_end; i++) {
		for(int j = 0; j <= j_end; j++) {
			sum += orig_table[i][j];
		}
	}	
	return sum;
}

void print_tab(int table[7][7], int w, int h) {
	for(int i = 0; i < h; i++) {
		for(int j = 0; j < w; j++) {
			fprintf(stderr, "%d\t", table[i][j]);
		}
		fprintf(stderr, "\n");
	}
}

int get_sum(int orig_table[7][7], int x, int y, int w, int h) {

	int sum = 0;
	for(int i = x; i <= i_end; i++) {
		for(int j = y; j <= j_end; j++) {
			sum += orig_table[i][j];
		}
	}	
	return sum;
}

int main() {
	int orig_table[7][7];
	for(int i = 0; i < 7; i++) {
		for(int j = 0; j < 7; j++) {
			orig_table[i][j] = i * 7 + j;
		}
	}

	int sum_table[7][7];
	/* Calculate sat */
	for(int i = 0; i < 7; i++) {
		for(int j = 0; j < 7; j++) {
			sum_table[i][j] = calc_sum(orig_table, i, j);
		}
	}

	int sum = get_sum(orig_table, 2, 2, 3, 3);

	/* Print table */
	fprintf(stderr, "Original table - \n");
	print_tab(orig_table, 7, 7);
	fprintf(stderr, "Sum table - \n");
	print_tab(sum_table, 7, 7);
}

