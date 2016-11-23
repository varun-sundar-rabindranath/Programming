#include <iostream>
using namespace std;

struct a {
	int b;
        a() {
		cout<<"a cons"<<endl;
	}
	~a(){
		cout<<"a dest "<<endl;
	}
};

int main() {
	{
		struct a obj;
	}
	return 0;
}
