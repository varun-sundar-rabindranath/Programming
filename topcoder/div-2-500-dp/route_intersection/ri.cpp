#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define MAX_MOVES 55


class RouteIntersection {
public : 
  string isValid(int N, vector<int> coords, string moves) {
    vector<vector<pair<int, int> > > dp(MAX_MOVES); 

    for(int i = 0; i < coords.size(); i++) {
      int dim = coords[i];
      int m = (moves[i] == '-') ? -1 : 1;
      for(int j = 0; j <= i; j++) {
	process_vector(dp[j], dim, m);
	if(dp[j].empty()) {
	  return "NOT VALID";
	}

      }
    }
    return "VALID";
  }

private : 
  void process_vector(vector<pair<int, int> > &dp, int dim, int move) {
    bool element_found = false;

    vector<pair<int, int> >::iterator it = dp.begin();

    while(it != dp.end()) {

      if(it->first == dim) {
	it->second += move;
	element_found = true;
      }
      if(it->first == dim && it->second == 0) {
	it = dp.erase(it);
      } else {
	it++;
      }

      if(element_found) { break; }
    }

    if(!element_found) {
      dp.push_back(pair<int, int>(dim, move));
    }
  }
};

