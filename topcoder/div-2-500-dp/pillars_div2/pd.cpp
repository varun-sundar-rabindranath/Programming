#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;


double rope_length(int base, int height) {
  return sqrt( base * base + height * height);
}

class PillarsDivTwo
{
public:
    double maximalLength(vector <int> height, int w)
    {
      double d1 = 0;
      double d2 = 0;

      for(int i = 0; i < height.size(); i += 2) {
	if(i != height.size() - 1)
	  d1 += rope_length(w, height[i] - 1);
	if(i != 0)
	  d1 += rope_length(w, height[i] - 1);
      }

      for(int i = 1; i < height.size(); i += 2) {
	if(i != height.size() - 1)
	  d2 += rope_length(w, height[i] - 1);
	if(i != 0)
	  d2 += rope_length(w, height[i] - 1);
      }

      return (max(d1, d2));

#if 0
      for(int i = 1; i < height.size(); i++) {
	if(i % 2 == 0) {
	  /* consider max */
	  d1 += rope_length(w, height[i] - 1);
	} else {
	  /* consider min */
	  d1 += rope_length(w, height[i - 1] - 1);
	}
      }

      for(int i = 1; i < height.size(); i++) {
	if(i % 2 == 1) {
	  /* consider min */
	  d2 += rope_length(w, height[i - 1] - 1);
	} else {
	  /* consider max */
	  d2 += rope_length(w, height[i] - 1);
	}
      }
#endif

      return max(d1, d2);
    }
};

