#include <stdio.h>
#include <stdlib.h>

int main() {
	char *a = (char *)calloc(10, sizeof(int));
	for(int i = 0; i < 10 * sizeof(int); i++) {
		a[i] = 1;
	}
	int * access_0 = (int *)a;
	int * access_0_plus_1 = (int *) (a + 1);
	fprintf(stderr, "access_0 %d \n", *access_0); 
	fprintf(stderr, "access_0_plus_1 %d \n", *access_0_plus_1);
}
