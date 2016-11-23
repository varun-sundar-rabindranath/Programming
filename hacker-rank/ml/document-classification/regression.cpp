#include <iostream>
#include <cmath>
#include <cassert>
using namespace std;

#include "regression.hpp"

#define INF 10000

#define ISZERO_LIMIT 1000

float hypothesis(vector<float> &x, vector<float> &theta) {
  double theta_t_x = 0;
  assert(x.size() == theta.size());
  for(int i = 0; i < x.size(); i++) {
    theta_t_x += theta[i] * x[i];
  }
  double sigmoid_ret_val =  ( 1 / ( 1 + exp(-theta_t_x) )); 

  return (sigmoid_ret_val >= 0.5) ? 1 : 0;
}

bool is_zero(float val) {
  int int_ret = val * ISZERO_LIMIT;
  return (int_ret == 0) ? true : false;
}

/* Given x (feature vectors) and y (known outputs) return
 * the weights calculated by gradient_descent;
 */
vector<float> gradient_descent(vector<vector<float> > &x, vector<float> &y) {

  static int count = 0;
  count++;

  vector<float> theta;

  int num_theta = x[0].size();
  int m = x.size();
  /* Initialize theta */
  for(int i = 0; i < num_theta; i++) {
    theta.push_back(1);
  }

  int iteration = 0;

  bool yet_to_converge = true;
  while(yet_to_converge) {

    yet_to_converge = false;

    vector<float> slopes;
    for(int i = 0; i < num_theta; i++) {
      /* Find slope of each theta */
      float slope = 0;
      for(int j = 0; j < m; j++) {
	slope += (hypothesis(x[j], theta) - y[j]) * x[j][i];
      }	
      slope /= (float)m;
      slope *= LEARNING_RATE;
      slopes.push_back(slope);

    }

#if 0
    if(iteration % 1 == 0) {
      for(int i = 0; i < num_theta; i++) {
	cout<<slopes[i]<<" ";
      }
      cout<<endl;
    }
#endif


    bool mess_printed = false; 
    for(int i = 0; i < num_theta; i++) {
      if(!is_zero(slopes[i])) { 
	if(!mess_printed) {
	  //cout<<"Getting thetas for "<<count<< " |  theta "<<i<<" not converged yet !! --> "<<slopes[i]<<endl; 
	  mess_printed = true;
	}
	yet_to_converge = true; 
      }
      theta[i] -= slopes[i];
    }

    iteration++;

  }

  return theta;
}


/* Debugging functions */
void print_iteration_vs_cost(int iteration, vector<float> &theta, vector<vector<float> > &x, vector<float> &y) {
  /* Calculate the cost wrt to this set of theta */
  int cost = 0;
  for(int i = 0; i < x.size(); i++) {
    if(hypothesis(x[i], theta) != y[i]) {
      cost += INF;
    }
  }

  fprintf(stderr, "%d %d\n", iteration, cost);
}
