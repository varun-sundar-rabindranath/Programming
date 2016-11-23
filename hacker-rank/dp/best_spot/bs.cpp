#include <iostream>
#include <vector>
#include <string> 
#include <climits>
#include <cassert>

using namespace std; 

int dp[505][505];

void fill_dp(int v) {
  for(int i = 0; i < 505; i++) {
    for(int j = 0; j < 505; j++) {
      dp[i][j] = v;
    }
  }
}

int get_cost(vector<vector<int> > &grid, vector<vector<int> > &best_spot, int x, int y, int curr_best) {
  assert(x != 0); assert(y != 0);
  if((grid[0].size() + 1) - x < best_spot[0].size()) { return INT_MAX; }
  if((grid.size() + 1) - y < best_spot.size()) { return INT_MAX; }

  int bs_dim_x = best_spot[0].size();
  int bs_dim_y = best_spot.size();

  int cost = 0;
  int bs_row = 0;
  int bs_col = 0;
  for(int i = (y - 1); i < (y - 1) + bs_dim_y; i++) {
    bs_col = 0;
    for(int j = (x - 1); j < (x - 1) + bs_dim_x; j++) {
      cost += (grid[i][j] - best_spot[bs_row][bs_col]) * (grid[i][j] - best_spot[bs_row][bs_col]);

      if(cost > curr_best) { return INT_MAX; }

      bs_col++;
    }
    bs_row++;
  }

  return cost;
}

int get_col_sum(vector<vector<int> > &square_grid, vector<vector<int> > &square_plan, int col_index, int y) {
    int col_sum = 0;
    for(int i = 0; i < square_plan.size(); i++) { 
      col_sum += square_grid[y + i][col_index];
    }
    return col_sum;
}

/* Return INT_MAX if x and y positions are not valid */
int get_asquare_val(vector<vector<int> > &square_grid, vector<vector<int> > &square_plan, int x, int y) {
  assert(x != 0); assert(y != 0);
  if((square_grid[0].size() + 1) - x < square_plan[0].size()) { return INT_MAX; }
  if((square_grid.size() + 1) - y < square_plan.size()) { return INT_MAX; }

  int index_x = x - 1;
  int index_y = y - 1;

  if(dp[y][x - 1] != -1) {
    /* get column sum */
    int col_index = index_x + square_plan[0].size() - 1;
    int col_sum_right = get_col_sum(square_grid, square_plan, col_index, index_y);
    col_index = index_x - 1;
    int col_sum_left = get_col_sum(square_grid, square_plan, col_index, index_y);
    
    dp[y][x] = dp[y][x - 1] + col_sum_right - col_sum_left;
    return dp[y][x];
  } else {
    int val = 0;
    for(int i = index_y; i < index_y + square_plan.size(); i++) {
      for(int j = index_x; j < index_x + square_plan[0].size(); j++) {
	val += square_grid[i][j];
      }
    }  
    dp[y][x] = val;
    return dp[y][x];
  }

}

int main() {

  fill_dp(-1);

  vector<vector<int> > grid, best_spot;

  int R, C, H, W;
  cin>>R;
  cin>>C;

  grid.resize(R);

  for(int i = 0; i < R; i++) {
    for(int j = 0; j < C; j++) {
      int val;
      cin>>val;
      grid[i].push_back(val);
    }
  }

  cin>>H;
  cin>>W;

  best_spot.resize(H);
  for(int i = 0; i < H; i++) {
    for(int j = 0; j < W; j++) {
      int val;
      cin>>val;
      best_spot[i].push_back(val);
    }
  }

  vector<vector<int> > square_grid, square_plan;
  square_grid.resize(R);
  for(int i = 0; i < R; i++) {
    for(int j = 0; j < C; j++) {
      int val = grid[i][j] * grid[i][j];
      square_grid[i].push_back(val);
    }
  }

  int plan_sum = 0;
  square_plan.resize(H);
  for(int i = 0; i < H; i++) {
    for(int j = 0; j < W; j++) {
      int val = best_spot[i][j] * best_spot[i][j];
      square_plan[i].push_back(val);
      plan_sum += val;
    }
  }


  int bx, by; int cost = INT_MAX;
  for(int i = 1; i <= R; i++) {
    for(int j = 1; j <= C; j++) {
      int val = get_asquare_val(square_grid, square_plan, j, i);
      cout<<i<<" "<<j<<" --> "<<val<<endl;
      if(val < cost) {
	bx = j; by = i; cost = val;
      }
    }
  }

  /* Square diff */
  int op = 0;
  int x_plan_iter = 0;
  int y_plan_iter = 0;
  for(int i = by - 1; i < by - 1 + square_plan.size(); i++) {
    x_plan_iter = 0;
    for(int j = bx - 1; j < bx - 1 + square_plan[0].size(); j++) {
      op += (grid[i][j] - best_spot[y_plan_iter][x_plan_iter]) * (grid[i][j] - best_spot[y_plan_iter][x_plan_iter]);
      x_plan_iter++;
    }
    y_plan_iter++;
  }
  cout<<op<<endl<<by<<" "<<bx<<endl;

#if 0
  /* Try out all positions :P */ 
  int bs_x = 0; int bs_y = 0; int cost = INT_MAX; 

  for(int i = 1; i <= R; i++) {
    for(int j = 1; j <= C; j++) {
      int this_cost = get_cost(grid, best_spot, j, i, cost);
      if(this_cost < cost) {
	cost = this_cost;
	bs_x = i;
	bs_y = j;
      }
    }
  }
  cout<<cost<<endl<<bs_x<<" "<<bs_y<<endl;
#endif


  return 0;
}
