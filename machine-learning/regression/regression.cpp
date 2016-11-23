#include <iostream>
#include <cmath>
#include <cassert>
using namespace std;

#include "regression.hpp"

#define INF 10000

float hypothesis(vector<float> &x, vector<double> &theta) {
  double theta_t_x = 0;
  assert(x.size() == theta.size());
  for(int i = 0; i < x.size(); i++) {
    theta_t_x += theta[i] * x[i];
  }
  double sigmoid_ret_val =  ( 1 / ( 1 + exp(-theta_t_x) )); 

  return (sigmoid_ret_val >= 0.5) ? 1 : 0;
}

/* Given x (feature vectors) and y (known outputs) return
 * the weights calculated by gradient_descent;
 */
vector<double> gradient_descent(vector<vector<float> > & x, vector<float> & y) {
  //TODO : Add support for linear regression as well
  
  fprintf(stderr, "WARNING : Your Infinity cost is 10000 \n");
  
  vector<double> theta;

  int num_theta = x[0].size();
  int num_features = x[0].size();
  int training_samples = x.size();

  /* Initialize all theta to 0 */
  for(int i = 0; i < num_theta; i++) {
    theta.push_back(1);
  }

  int iteration = 0;

  bool thetas_yet_to_reach_min = true;
  while(thetas_yet_to_reach_min) {

    thetas_yet_to_reach_min = false;

    vector<double> updated_theta;

    for(int i = 0; i < num_theta; i++) {
      double theta_i_slope = 0;
      for(int j = 0; j < training_samples; j++) {
      	theta_i_slope += (hypothesis(x[j], theta) - y[j]) * x[j][i];
      }

      updated_theta.push_back(LEARNING_RATE * theta_i_slope);
    }

    /* Simultaneous update */
    for(int i = 0; i < num_theta; i++) {
      theta[i] -= updated_theta[i];
      if(updated_theta[i] != 0) {
	thetas_yet_to_reach_min = true;
      }
    }

#ifdef DEBUG
    print_iteration_vs_cost(iteration++, theta, x, y);
#endif
  }

  return theta;
}


/* Debugging functions */
void print_iteration_vs_cost(int iteration, vector<double> &theta, vector<vector<float> > &x, vector<float> &y) {
  /* Calculate the cost wrt to this set of theta */
  int cost = 0;
  for(int i = 0; i < x.size(); i++) {
    if(hypothesis(x[i], theta) != y[i]) {
      cost += INF;
    }
  }

  fprintf(stderr, "%d %d\n", iteration, cost);
}
