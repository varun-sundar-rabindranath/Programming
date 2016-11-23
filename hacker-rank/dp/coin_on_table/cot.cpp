#include <iostream>
#include <cassert>
using namespace std;

#define N 55
#define M 55
#define K 1005

#define COT_INF 10000

char board[N][M] = {0};
int dp[N][M][K] = {0};
int current_n = 0;
int current_m = 0;

int get_new_pos_i(int m, int old_posi) {
  switch(m) {
    case 0  : return old_posi - 1;
    case 1  : return old_posi + 1;
    case 2  : return old_posi;
    case 3  : return old_posi; 
    default : assert(0);
  }
}

int get_new_pos_j(int m, int old_posj) {
  switch(m) {
    case 0  : return old_posj;
    case 1  : return old_posj;
    case 2  : return old_posj - 1;
    case 3  : return old_posj + 1; 
    default : assert(0);
  }
}

bool pos_valid(int posi, int posj) {
  if(posi < current_n && posj < current_m &&
     posi >= 0 && posj >= 0) {
    return true;
  } else {
    return false;
  }
}

int rec(char b[N][M], int posi, int posj, int k) {

  if(!pos_valid(posi, posj)) {
    return COT_INF;
  }
  
  if(k < 0) {
    return COT_INF;
  }

  if((int)b[posi][posj] == 4) {
    return 0;
  }
  
  int move = (int)b[posi][posj];

  int c1i = get_new_pos_i( (move + 0) % 4, posi); 
  int c2i = get_new_pos_i( (move + 1) % 4, posi); 
  int c3i = get_new_pos_i( (move + 2) % 4, posi); 
  int c4i = get_new_pos_i( (move + 3) % 4, posi); 

  int c1j = get_new_pos_j( (move + 0) % 4, posj);  
  int c2j = get_new_pos_j( (move + 1) % 4, posj);
  int c3j = get_new_pos_j( (move + 2) % 4, posj);
  int c4j = get_new_pos_j( (move + 3) % 4, posj);

  int cost1 = COT_INF, cost2 = COT_INF, cost3 = COT_INF, cost4 = COT_INF;

  if(pos_valid(c1i, c1j) && k > 0) {
    if(dp[c1i][c1j][k - 1] == -1) {
      dp[c1i][c1j][k - 1] = rec(b, c1i, c1j, k - 1);
    }
    cost1 = dp[c1i][c1j][k - 1] + 0;
  }

  if(pos_valid(c2i, c2j) && k > 0) {
    if(dp[c2i][c2j][k - 1] == -1) {
      dp[c2i][c2j][k - 1] = rec(b, c2i, c2j, k - 1);
    }
    cost2 = dp[c2i][c2j][k - 1] + 1;
  }

  if(pos_valid(c3i, c3j) && k > 0) {
    if(dp[c3i][c3j][k - 1] == -1) {
      dp[c3i][c3j][k - 1] = rec(b, c3i, c3j, k - 1);
    }
    cost3 = dp[c3i][c3j][k - 1] + 1;
  }

  if(pos_valid(c4i, c4j) && k > 0) {
    if(dp[c4i][c4j][k - 1] == -1) {
      dp[c4i][c4j][k - 1] = rec(b, c4i, c4j, k - 1);
    }
    cost4 = dp[c4i][c4j][k - 1] + 1;
  }

  return (min(min(cost1, cost2), min(cost3, cost4)));
}

int main() {

  /* Init dp */
  for(int i = 0; i < N; i++) {
    for(int j = 0; j < M; j++) {
      board[i][j] = -1;
      for(int k = 0; k < K; k++) {
	dp[i][j][k] = -1; 
      }
    }
  }


  int n, m, k;
  cin>>n;
  cin>>m;
  cin>>k;

//  n -= 1;
//  m -= 1;

  current_n = n;
  current_m = m;

  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      char m;
      cin>>m;

      switch(m) {
	case 'U' :  board[i][j] = 0;
		    break;
	case 'D' :  board[i][j] = 1;
		    break;
	case 'L' :  board[i][j] = 2;
		    break;
	case 'R' :  board[i][j] = 3;
		    break;
	case '*' :  board[i][j] = 4;
		    break;
	default  : assert(0);
      }

    }
  }

  int ret_val = rec(board, 0, 0, k);
  if(ret_val >= COT_INF) {
    cout<<"-1"<<endl;
  } else {
    cout<<ret_val<<endl;
  }

  return 0;
}
