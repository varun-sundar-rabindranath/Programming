#include <iostream>
using namespace std;

typedef struct a {
	int b;

        a(int b_) {
          b = b_;
	}
        a() {
		cout<<"a cons"<<endl;
	}
	~a(){
		cout<<"a dest "<<endl;
	}
        int wow() {
          return b;
        }
        a operator+ (struct a a_) {
          return a(this->b + a_.b);
        }
}a;

int main() {
	{
		struct a obj;
                cout<<"wow() - "<<obj.wow()<<endl;

                struct a add_1(3);
                struct a add_2(5);

                struct a sum = add_1 + add_2;

                cout<<"sum - "<<sum.wow()<<endl;
	}
	return 0;
}
