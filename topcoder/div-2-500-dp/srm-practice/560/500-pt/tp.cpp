#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class TomekPhone {
public :
  int minKeyStrokes(vector<int> f, vector<int> ks) {

    /* is it possible */
    int total_letters = f.size();
    int can_acc = 0;

    for(int i = 0; i < ks.size(); i++) {
      can_acc += ks[i];
    }

    if(total_letters > can_acc) { return -1; }

    vector<int> sf = f;
    vector<int> sks = ks;
    sort(sf.begin(), sf.end());
    sort(sks.begin(), sks.end());
    reverse(sf.begin(), sf.end());
    reverse(sks.begin(), sks.end());

    vector<int> kstrokes(sf.size(), 0);
    vector<int> assigned(ks.size(), 0);
    
    int marker = 0;

    for(int i = 0; i < sf.size(); i++) {
      /* Find marker position */
      while(assigned[marker % ks.size()] == sks[marker % ks.size()]) {
	marker++;
      }

      kstrokes[i] = assigned[marker % ks.size()] + 1;
      assigned[marker % ks.size()]++;
      marker++;
    }

    int retval = 0;
    for(int i = 0; i < sf.size(); i++) {
      retval += kstrokes[i] * sf[i];
    }

    return retval;


  }
};
