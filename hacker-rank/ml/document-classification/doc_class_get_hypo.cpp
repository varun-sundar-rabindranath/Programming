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

  int num_feature_words;
  int x_height, x_width;
  int num_y;

  vector<string> feature_words;

  /* Read data from input */
  cin>>num_feature_words;
  for(int i = 0; i < num_feature_words; i++) {
      string s;
      cin>>s;
      feature_words.push_back(s);
  }

  cin>>x_height;
  cin>>x_width;

  for(int i = 0; i < x_height; i++) {
    vector<float> fv;
    /* Add for theta 0 */
    fv.push_back(1);
    for(int j = 0; j < x_width; j++) {
      float f;
      cin>>f;
      fv.push_back(f);
    }
    x.push_back(fv);
  }

  cin>>num_y;
  for(int i = 0; i < num_y; i++) {
    int f;
    cin>>f;
    y.push_back(f);
  }

  fprintf(stderr, " m --> %lu <-- \n", x.size());
  fprintf(stderr, " x --> %lu <-- \n", x[0].size());
  fprintf(stderr, " y --> %d <-- \n", num_y);

#if 0
  for(int i = 0; i < SAMPLES; i++) {
    vector<float> fv;
    fv.push_back(1);
    for(int j = 0; j < FEATURES; j++) {
      fv.push_back((i + 1) * 10);
    }
    x.push_back(fv);

    if(i < SAMPLES / 2) { y.push_back(0); } else { y.push_back(1); }
  }
#endif

  for(int i = 1; i <= 8; i++) {
    vector<float> new_y;
    for(int j = 0; j < num_y; j++) {
      if(i == y[j]) {
	new_y.push_back(1);
      } else {
	new_y.push_back(0);
      }
    }

    vector<float> theta;
    theta = gradient_descent(x, new_y);

    /* This hypo theta */
    cout<<"Class "<<i<<" theta : "<<endl;
    for(int j = 0; j < theta.size(); j++) {
      cout<<theta[j]<<" ";
    }
    cout<<endl;

    /* Estimating correctness */
    int correct_estimation = 0;
    int incorrect_estimation = 0;
    for(int i = 0; i < num_y; i++) {
      if(hypothesis(x[i], theta) == new_y[i]) {
        correct_estimation++;
      } else {
        incorrect_estimation++;
      }
    }

    fprintf(stderr, "%f percent of %d samples estimated correctly \n", (float)((float)correct_estimation / (float)num_y) * 100, num_y);
  }

#if 0
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
#endif

  return 0;
}
