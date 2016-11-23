#include <iostream>
#include <set>
#include <cassert>
using namespace std;
int main() {
    set<int> int_set;
    int_set.insert(10);
    int_set.insert(20);
    int_set.insert(30);
    int_set.insert(40);

    pair<set<int>::iterator, bool> ret;
    ret = int_set.insert(10);
    cout<<"Ret ins 10 "<<ret.second<<endl;
    ret = int_set.insert(5000);
    cout<<"Ret ins 5000 "<<ret.second<<endl;

    return 0;
}
