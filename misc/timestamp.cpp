#include <iostream>
#include <string>
#include <cassert>
#include <cstring>
#include <cstdio>

using namespace std;

string get_as_timestamp(double seconds) {

  double s = seconds;
  int hr = 0, min = 0, sec = 0, ms = 0;

  while(s >= 3600) {
    hr++;
    s -= 3600;
  }

  while(s >= 60) {
    min++;
    s -= 60;
  }

  sec = int(s);

  ms = (int)((seconds - (double)((int)seconds)) * 1000);

  assert(min < 60);
  assert(sec < 60);

  char c_timestamp[100];
  sprintf(c_timestamp, "%05d:%02d:%02d.%03d", hr, min, sec, ms);

  return string(c_timestamp);
}

int main() {

  for(int i = 0; i < 7500; i++) {
    cout<<i<<" - "<<get_as_timestamp((float)i + 0.92334)<<endl;
  }

  return 0;
}

