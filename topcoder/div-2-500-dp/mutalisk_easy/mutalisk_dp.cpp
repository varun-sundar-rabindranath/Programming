#include<iostream>
#include <vector>
#include <climits>
using namespace std;

int dp[65][65][65];

void init_dp(int val) {
  for(int i = 0; i < 65; i++) {
    for(int j = 0; j < 65; j++) {
      for(int k = 0; k < 65; k++) {
	dp[i][j][k] = val;
      }
    }
  }
}

#define dp_access(v) (dp[v[0]][v[1]][v[2]])

vector<int> selection(vector<int> hp, int a, int b, int c) {
  hp[a] -= 9;
  hp[b] -= 3;
  hp[c] -= 1;

  hp[a] = (hp[a] < 0) ? 0 : hp[a];
  hp[b] = (hp[b] < 0) ? 0 : hp[b];
  hp[c] = (hp[c] < 0) ? 0 : hp[c];
  return hp;
}

bool target_reached(vector<int> hp) {
  if(hp[0] <= 0 && hp[1] <= 0 && hp[2] <= 0) {
    return true;
  } else {
    return false;
  }
}

bool valid_vec(vector<int> hp) {
  int num_pos = 0;
  for(int i = 0; i < hp.size(); i++) {
    if(hp[i] > 0) { num_pos++; }
  }

  if(num_pos == 1) {
    if(hp[0] > 0 && hp[1] <= 0 && hp[2] <= 0) { return true; }
    else				      { return false; }
  }
  if(num_pos == 2) {
    if(hp[0] > 0 && hp[1] > 0 && hp[2] <= 0) { return true; }
    else				     { return false; }
  }
  return true;
}

class MutaliskEasy {
public : 
  int minimalAttacks(vector<int> x) {
    init_dp(-1);
    if(x.size() == 1) { x.push_back(0); x.push_back(0); }
    if(x.size() == 2) { x.push_back(0); }
    return rec(x);
  }
private :
  int rec(vector<int> x) {

    if(target_reached(x)) { return 0; }

    int cost1 = INT_MAX, cost2 = INT_MAX, cost3 = INT_MAX, cost4 = INT_MAX, cost5 = INT_MAX, cost6 = INT_MAX;

    vector<int> vec1, vec2, vec3, vec4, vec5, vec6;

    vec1 = selection(x, 0, 1, 2);
    vec2 = selection(x, 0, 2, 1);
    vec3 = selection(x, 1, 0, 2);
    vec4 = selection(x, 1, 2, 0);
    vec5 = selection(x, 2, 0, 1);
    vec6 = selection(x, 2, 1, 0);

    if(target_reached(vec1) ||
       target_reached(vec2) ||
       target_reached(vec3) ||
       target_reached(vec4) ||
       target_reached(vec5) ||
       target_reached(vec6)) 
      { return 1;}

    if(dp_access(vec1) == -1) {
      dp_access(vec1) = rec(vec1) + 1;
    } 

    if(dp_access(vec2) == -1) {
      dp_access(vec2) = rec(vec2) + 1;
    } 

    if(dp_access(vec3) == -1) {
      dp_access(vec3) = rec(vec3) + 1;
    } 

    if(dp_access(vec4) == -1) {
      dp_access(vec4) = rec(vec4) + 1;
    } 

    if(dp_access(vec5) == -1) {
      dp_access(vec5) = rec(vec5) + 1;
    } 

    if(dp_access(vec6) == -1) {
      dp_access(vec6) = rec(vec6) + 1;
    } 

    cost1 = dp_access(vec1);
    cost2 = dp_access(vec2);
    cost3 = dp_access(vec3);
    cost4 = dp_access(vec4);
    cost5 = dp_access(vec5);
    cost6 = dp_access(vec6);

    int min1 = min(cost1, cost2);
    int min2 = min(cost3, cost4);
    int min3 = min(cost5, cost6);

    return min(min(min1, min2), min3);
  }
};

vector<int> get_vec(int a = 0, int b = 0, int c = 0) {
  vector<int> rv;
  if(a != 0) 
    rv.push_back(a);
  if(b != 0)
    rv.push_back(b);
  if(c != 0)
    rv.push_back(c);
  return rv;
}

int main() {
  MutaliskEasy m;
  cout<<m.minimalAttacks(get_vec(12, 10, 4))<<endl;
  cout<<m.minimalAttacks(get_vec(54, 18, 6))<<endl;
  cout<<m.minimalAttacks(get_vec(55, 60, 53))<<endl;
  cout<<m.minimalAttacks(get_vec(1, 1, 1))<<endl;
  cout<<m.minimalAttacks(get_vec(60, 40))<<endl;
  cout<<m.minimalAttacks(get_vec(60))<<endl;
  return 0;
}
