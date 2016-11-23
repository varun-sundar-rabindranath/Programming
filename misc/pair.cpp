#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

bool sort_op(const pair<string, int> &left, const pair<string, int> &right) {
  return left.second < right.second;
}


int main() {
  vector<pair<string, int> > stoi;

  stoi.push_back(pair<string, int>("wowo", 10));
  stoi.push_back(pair<string, int>("a", 10));
  stoi.push_back(pair<string, int>("b", 1));
  stoi.push_back(pair<string, int>("c", 9));
  stoi.push_back(pair<string, int>("d", 5));

  sort(stoi.begin(), stoi.end(), sort_op);

  for(int i = 0; i < stoi.size(); i++) {
    cout<<stoi[i].first<<" --> "<<stoi[i].second<<endl;
  }

  return 0;

}
