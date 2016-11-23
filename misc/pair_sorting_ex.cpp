#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
  vector<pair< int, pair<int, int> > > pair_vec;

  pair_vec.push_back( pair< int,pair<int, int> > (1 ,pair<int, int> (5, 5) ));
  pair_vec.push_back( pair< int,pair<int, int> > (1 ,pair<int, int> (4, 4) ));
  pair_vec.push_back( pair< int,pair<int, int> > (1 ,pair<int, int> (3, 3) ));
  pair_vec.push_back( pair< int,pair<int, int> > (4 ,pair<int, int> (2, 2) ));
  pair_vec.push_back( pair< int,pair<int, int> > (5 ,pair<int, int> (1, 1) ));

  sort(pair_vec.begin(), pair_vec.end());

  for(int i = 0; i < pair_vec.size(); i++){
    cout<<pair_vec[i].first<<" "<<pair_vec[i].second.first<<endl;
  }

  return 0;
}
