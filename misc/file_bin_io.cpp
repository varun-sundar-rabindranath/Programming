#include <iostream>
#include <cstdio>
using namespace std;

#define NUM_ELEMENTS 10

int main() {
    int write_file[NUM_ELEMENTS];
    int read_file[NUM_ELEMENTS];

    /* Init write file */
    for(int i = 0; i < NUM_ELEMENTS; i++) {
	write_file[i] = i*1000 * (-1);
    }

    /* write to file */
    FILE *fp_write = fopen("./write_bin", "wb");
    fwrite(write_file, sizeof(char), NUM_ELEMENTS * sizeof(int), fp_write);
    fclose(fp_write);

    /* Read from file */
    FILE *fp_read = fopen("./write_bin", "rb");
    fread(read_file, sizeof(char), NUM_ELEMENTS * sizeof(int), fp_read);
    fclose(fp_read);

    /* Print read file */
    for(int i = 0; i < NUM_ELEMENTS; i++) {
	fprintf(stderr, "%d ", read_file[i]);
    }
    fprintf(stderr, "\n");
    return 0;
}
