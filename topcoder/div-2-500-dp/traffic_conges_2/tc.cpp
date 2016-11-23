#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
using namespace std;

#define LL long long int

//#define LEAVES(h) (pow(2, h));

LL LEAVES(int h) {
  LL mask = 1;
  return (mask << h);
}

class TrafficCongestionDivTwo
{
public:
    long long theMinCars(int treeHeight)
    {
      LL new_spawn = 0;
      LL duds = 0;
      
      for(int i = 0; i <= treeHeight; i++) {
	LL new_leaves = LEAVES(i);
	
	new_leaves -= new_spawn * 2;

	new_leaves -= duds * 2;

	duds += new_spawn;

	assert(new_leaves >= 0);

	new_spawn = new_leaves;
      }

      return (new_spawn + duds);
    }
};

int main() {
  TrafficCongestionDivTwo t;

  cout<<t.theMinCars(1)<<endl;
  cout<<t.theMinCars(2)<<endl;
  cout<<t.theMinCars(3)<<endl;
  cout<<t.theMinCars(10)<<endl;
  cout<<t.theMinCars(60)<<endl;

  return 0;
}
