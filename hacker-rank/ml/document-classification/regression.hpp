#ifndef __REGRESSION_H__
#define __REGRESSION_H__

#include <vector>
#include <cstdio>
using namespace std;

#define LEARNING_RATE 0.01

#define DEBUG

/* Interface file to linear/logistic regression functions */

/* Given x (feature vectors) and y (known outputs) return
 * the weights calculated by gradient_descent;
 */
vector<float> gradient_descent(vector<vector<float> > & x, vector<float> & y);

/* Hypothesis function */
float hypothesis(vector<float> &x, vector<float> &theta);

/* Debugging functions */
void print_iteration_vs_cost(int iteration, vector<float> &theta, vector<vector<float> > &x, vector<float> &y);

#endif // __REGRESSION_H__
