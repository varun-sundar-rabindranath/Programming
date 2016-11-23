#include <iostream>
#include <vector>
using namespace std;

#define LEARNING_RATE 0.01

#define ISZERO_LIMIT 1000

bool is_zero(float val) {
  int int_ret = val * ISZERO_LIMIT;
  return (int_ret == 0) ? true : false;
}

float hypothesis(vector<float> x, vector<float> theta) {
  float price = 0;

  int num_elements = x.size();
  int four_boundary = (x.size() / 4) * 4;

  for(int i = 0; i < four_boundary; i = i + 4) {
    price += x[i + 0] * theta[i + 0];
    price += x[i + 1] * theta[i + 1];
    price += x[i + 2] * theta[i + 2];
    price += x[i + 3] * theta[i + 3];
  }

  for(int i = four_boundary; i < num_elements; i++) {
    price += x[i] * theta[i];
  }

#if 0
  for(int i = 0; i < x.size(); i++) {
    price += x[i] * theta[i]; 
  }
#endif
  return price;
}

void cost(int iteration, vector<vector< float> > &x, vector<float> &y, vector<float> &theta) {

  int c = 0;
  for(int i = 0; i < x.size(); i++) {
    float pred_price = hypothesis(x[i], theta);
    c += (pred_price - y[i]) * (pred_price - y[i]);
  }
  c /= x.size();
  cout<<"Iteration - "<<iteration<<" | Cost - "<<c<<endl;
}

vector<float> gradient_descent(vector<vector<float> > &x, vector<float> &y) {
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

    //cost(iteration, x, y, theta);

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

      if(i == 0 && iteration % 1 == 0) {
        cout<<"theta 0 - "<<theta[0]<<" | Slope of theta 0 - "<<slope<<endl;
      }
    }

    for(int i = 0; i < num_theta; i++) {
      if(!is_zero(slopes[i])) { yet_to_converge = true; }
      theta[i] -= slopes[i];
    }

    iteration++;

  }

  return theta;
}

void print_xy(vector<vector<float> > &x, vector<float> &y) {
  for(int i = 0; i < x.size(); i++) {
    for(int j = 0; j < x[0].size(); j++) {
      cout<<x[i][j]<<" ";
    }
    cout<<" --> "<<y[i]<<endl;
  }
}

int main() {
  int F, N;
  cin>>F;
  cin>>N;

  vector<vector<float> > x;
  vector<float> y;

  for(int i = 0; i < N; i++) {
    vector<float> f;

    /* Add bias for theta 0 */
    f.push_back(1);

    for(int j = 0; j < F; j++) {
      float val;
      cin>>val;
      f.push_back(val);
    }
    x.push_back(f);

    float price;
    cin>>price;
    y.push_back(price);
  }

//  print_xy(x, y);

//  cout<<" M - "<<x.size()<<endl;
//  cout<<" F - "<<x[0].size()<<endl;
  int T;
  cin>>T;

  vector<vector<float> > x_query;
  vector<float> y_query;

  for(int i = 0; i < T; i++) {
    vector<float> f;
    f.push_back(1);
    for(int j = 0; j < F; j++) {
      float val;
      cin>>val;
      f.push_back(val);
    }
    x_query.push_back(f);
  }

  vector<float> theta = gradient_descent(x, y);

#if 0
  /* Print theta */
  cout<<"Theta \n ";
  for(int i = 0; i < theta.size(); i++) {
    cout<<theta[i]<<" ";
  }
  cout<<endl;

  /* Print x */
  for(int i = 0; i < x_query.size(); i++) {
    for(int j = 0; j < x_query[0].size(); j++) {
      cout<<x_query[i][j]<<" ";
    }
    cout<<endl;
  }
#endif
  
  for(int i = 0; i < x_query.size(); i++) {
    cout<<hypothesis(x_query[i], theta)<<endl;
  }

  return 0;
}
