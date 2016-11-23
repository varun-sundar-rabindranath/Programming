#include <iostream>
using namespace std;

enum tex_ref {
  tex_1 = 0,
  tex_2 = 1,
  tex_3 = 2,
  tex_4 = 3,
  tex_5 = 4,
};

int get_sum() {
  int tex_1_mem = 5;
  int tex_2_mem = 5;
  int tex_3_mem = 5;
  int tex_4_mem = 5;
  int tex_5_mem = 5;

  return (tex_1 + tex_2 + tex_3 + tex_4 + tex_5);

}

int main() {

  cout<<get_sum()<<endl;
  return 0;

}
