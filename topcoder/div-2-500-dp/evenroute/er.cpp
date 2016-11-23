#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

int dist[50][50] = {0};
int zero_dist[50] = {0};

class EvenRoute {
public:
    string isItPossible(vector <int> x, vector <int> y, int wantedParity) {

      vector<pair<bool, bool> > ending;
      ending.resize(x.size());     

      for(int i = 0; i < x.size(); i++) {
	zero_dist[i] = abs(x[i]) + abs(y[i]);
	for(int j = 0; j < y.size(); j++) {
	  dist[i][j] = abs(x[i] - x[j]) + abs(y[i] - y[j]);
	}
      }

      for(int i = 0; i < x.size(); i++) {
      }

	  
	  
    }
};


