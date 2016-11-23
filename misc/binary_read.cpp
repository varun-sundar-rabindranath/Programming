#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NUM_ELEMENTS 6000

/* Declare struct to read and write */
typedef struct example {
    char a;
    double b;
    int c, d;
    float e;
}example;
    
int main() {

    /* Create example objects */
    example *ex_obj = NULL;
    ex_obj = (example *)calloc(NUM_ELEMENTS, sizeof(example));
    assert(ex_obj != NULL && "Can't calloc memory");
    
    /* Init example objects */
    for(int i = 0; i < NUM_ELEMENTS; i++) {
	ex_obj[i].a = 'a';
	ex_obj[i].b = i;
	ex_obj[i].c = i + 1;
	ex_obj[i].d = i + 2;
	ex_obj[i].e = i + 3;
    }

    /* Write mem to file */
    FILE *fp = NULL;
    fp = fopen("./bin_file", "wb");
    assert(fp != NULL);
    assert(fwrite(ex_obj, sizeof(char), NUM_ELEMENTS * sizeof(example), fp) == NUM_ELEMENTS * sizeof(example));
    fclose(fp);

    /* Read as char from the bin file*/
    char *char_array = NULL;
    char_array = (char *)calloc(NUM_ELEMENTS * sizeof(example), sizeof(char));
    assert(char_array != NULL && "cant calloc memory");

    fp = NULL;
    fp = fopen("./bin_file", "rb");
    assert(fp != NULL);
    assert(fread(char_array, sizeof(char), NUM_ELEMENTS * sizeof(example), fp) == NUM_ELEMENTS * sizeof(example));
    fclose(fp);

    /* Compare elements */
    for(int i = 0; i < NUM_ELEMENTS; i++) {
	example *read_ex_obj = (example *)char_array;

	//fprintf(stderr, "index %d | exobj - %c %lf %d %d %f | ", i, ex_obj[i].a, ex_obj[i].b, ex_obj[i].c, ex_obj[i].d, ex_obj[i].e);
	//fprintf(stderr, "read exobj - %c %lf %d %d %f \n ", read_ex_obj[i].a, read_ex_obj[i].b, read_ex_obj[i].c, read_ex_obj[i].d, read_ex_obj[i].e);
	assert(ex_obj[i].a == read_ex_obj[i].a);
	assert(ex_obj[i].b == read_ex_obj[i].b);
	assert(ex_obj[i].c == read_ex_obj[i].c);
	assert(ex_obj[i].d == read_ex_obj[i].d);
	assert(ex_obj[i].e == read_ex_obj[i].e);
    }

    free(ex_obj);
    free(char_array);
    return 0;

}
