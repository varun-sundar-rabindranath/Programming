#include <iostream>
#include <cmath> 
#include <vector>
using namespace std;

#define PI 3.14159265

vector<float> get_new_point(int x1, int y1, int x2, int y2) {
  vector<float> ret_point;
  double angle = (atan2((y2 - y1), (x2 - x1)));
  ret_point.push_back(10 + 10 * cos(angle));
  ret_point.push_back(10 * sin(angle));
  return ret_point;
}

int main() {

  int x, y;
  x = 0;
  y = 0;

#if 0
  for(int i = 0; i < 360; i++) {
    cout<<cos(i * PI / 180)<<" "<<sin(i * PI / 180)<<endl;
  }
#endif


  vector<float> p1 = get_new_point(10, 0, 20, 0);
  vector<float> p2 = get_new_point(10, 0, 17, 7);
  vector<float> p3 = get_new_point(10, 0, 10, 10);
  vector<float> p4 = get_new_point(10, 0, 3, 7);
  vector<float> p5 = get_new_point(10, 0, 0, 0);
  vector<float> p6 = get_new_point(10, 0, 3, -7);
  vector<float> p7 = get_new_point(10, 0, 10, -10);
  vector<float> p8 = get_new_point(10, 0, 17, -7);

  cout<<p1[0]<<" "<<p1[1]<<endl;
  cout<<p2[0]<<" "<<p2[1]<<endl;
  cout<<p3[0]<<" "<<p3[1]<<endl;
  cout<<p4[0]<<" "<<p4[1]<<endl;
  cout<<p5[0]<<" "<<p5[1]<<endl;
  cout<<p6[0]<<" "<<p6[1]<<endl;
  cout<<p7[0]<<" "<<p7[1]<<endl;
  cout<<p8[0]<<" "<<p8[1]<<endl;


  return 0;

}
