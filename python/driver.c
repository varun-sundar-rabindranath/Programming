#include <Python.h>
#include <stdio.h>

#include "calculator.h"

int main() {
    Py_Initialize();
    initcalculator();
    printf("%d + %d - %d \n", 7, 3, add("json-encode.py\0", 7, 3));
    printf("%d - %d - %d \n", 7, 3, sub("json-encode.sy\0", 7, 3));
    Py_Finalize();
    return 0;
}
