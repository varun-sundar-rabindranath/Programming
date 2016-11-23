#include <iostream>
#include <stdio.h>
using namespace std;

class base_class {
public :
    void function_print_one() {
	printf("base - 1\n");
    }
    void function_print() {
	function_print_one();
    }
};
class derived_class : public base_class{
public :
    void function_print_one() {
	printf("derived - 1 \n");
    }
};

int main() {
    base_class base;
    derived_class derived;
    derived.function_print();
    return 0;
}
