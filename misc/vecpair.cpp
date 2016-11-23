#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
 vector<pair<int, int> > v;

 v.push_back(pair<int, int> (5, 2));
 v.push_back(pair<int, int> (4, 1));
 v.push_back(pair<int, int> (3, 4));
 v.push_back(pair<int, int> (2, 3));
 v.push_back(pair<int, int> (1, 5));

 sort(v.begin(), v.end());

 for(int i = 0; i < v.size(); i++) {
  cout<<v[i].first <<" "<<v[i].second<<endl;
 }

 return 0;
}
