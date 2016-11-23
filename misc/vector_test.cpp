#include<iostream>
#include<vector>

using namespace std;

int main() {
    vector<int> v1, v2;
    for(int i = 0; i < 10; i++) {
	v1.push_back(i);
    }
    v2 = v1;
    v1[5] = 10000;
    for(int i = 0; i < 10; i++) {
	cout<<v1[i]<<"  "<<v2[i]<<endl;
    }
    return 0;
}
