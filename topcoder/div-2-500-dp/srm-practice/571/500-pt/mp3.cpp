#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class FoxAndMp3Easy {
public : 
  vector<string> playList(int n) {
    vector<float> values;

    float num_digits = 1;
    for(int i = 1; i <= n; i++) {
      if(i == 10) { num_digits = 2; }
      if(i == 100) { num_digits = 3; }
      if(i == 1000) { num_digits = 4; }
      float fv = (float)i;
      fv /= (num_digits * 10); 
      values.push_back(fv);
    }

    sort(values.begin(), values.end());

    int r = (values.size() > 50) ? 50 : values.size();

    vector<string> ret;
    /* Create string vector */
    for(int i = 0; i < r; i++) {
      string s  = to_string((int) values[i] * 1000);
      s = strip_zeros(s);
      ret.push_back(s);
    }

    vector<string> answers;
    answers.push_back(ret[0] + ".mp3");

    int rep = 0;
    for(int i = 1; i < r; i++) {
      if(ret[i] != ret[i - 1]) {
	rep = 0;
	answers.push_back(ret[i] + ".mp3");
      } else {
	rep++;
	answers.push_back(append_n_zeros(n, ret[i]) + ".mp3");
      }
    }

    return answers;
  }
private :
  string strip_zeros(string s) {
    bool seen_non_zero = false;
    string r = "";
    for(int i = s.length() - 1; i >= 0; i--) {
      if(!seen_non_zero) {
	if(s[i] != '0') {
	  seen_non_zero = true;	
      	} else {
	  continue;
	}
      }

      if(seen_non_zero) {
	r += s[i];
      }
    }

    /* reverse r */
    for(int i = 0; i < r.length() / 2; i++) {
      char tmp = r[i];
      r[i] = r[r.length() - i];
      r[r.length() - i] = tmp;
    }

    return r;
  }

  string append_n_zeros(int n, string s) {
    string r = s;
    for(int i = 0; i < n; i++) {
      r += '0';
    }

    return r;
  }

};
