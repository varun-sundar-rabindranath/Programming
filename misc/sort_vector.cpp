#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
  vector<int> wow;

  wow.push_back(10);
  wow.push_back(15);
  wow.push_back(5);

  sort(wow.begin(), wow.end());

  for(int i = 0; i < wow.size(); i++) {
    cout<<wow[i]<<" ";
  }
  cout<<endl;

  if(binary_search(wow.begin(), wow.end(), 11)) {
    cout<<"found "<<endl;
  } else {
    cout<<"not found "<<endl;
  }

  return 0;
}
