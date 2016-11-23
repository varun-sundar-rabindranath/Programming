#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

#include "regression.hpp"

#define SAMPLES 6
#define FEATURES 1


void print_xy(vector<vector<float> > x, vector<float> y) {
  for(int i = 0; i < x[0].size(); i++) {
    fprintf(stderr, "f%d ", i + 1);
  }
  fprintf(stderr, "\n");
  for(int i = 0; i < x.size(); i++) {
    for(int j = 0; j < x[i].size(); j++) {
     fprintf(stderr, "%d ", (int)x[i][j]);
    }
    fprintf(stderr, " --> %d \n", (int)y[i]);
  }
}

int main() {
  vector<vector<float> > x;
  vector<float> y;

  for(int i = 0; i < SAMPLES; i++) {
    vector<float> fv;
    fv.push_back(1);
    for(int j = 0; j < FEATURES; j++) {
      fv.push_back((i + 1) * 10);
    }
    x.push_back(fv);

    if(i < SAMPLES / 2) { y.push_back(0); } else { y.push_back(1); }
  }

  print_xy(x, y);

  vector<double> theta;
  theta = gradient_descent(x, y);

  /* Estimating correctness */
  int correct_estimation = 0;
  int incorrect_estimation = 0;
  for(int i = 0; i < SAMPLES; i++) {
    if(hypothesis(x[i], theta) == y[i]) {
      correct_estimation++;
    } else {
      incorrect_estimation++;
    }
  }

  fprintf(stderr, "%d percent of samples estimated correctly \n", (correct_estimation / SAMPLES) * 100);

  return 0;
}
