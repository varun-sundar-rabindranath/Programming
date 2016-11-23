#include <iostream>
#include <vector>
#include <string> 
using namespace std;

#define TOTAL_CONFIG 1048576

int dp[TOTAL_CONFIG];

void fill_dp(int val) {
  for(int i = 0; i < TOTAL_CONFIG; i++) {
    dp[i] = val;
  }
}

class EllysCheckers {
public :
  string getWinner(string board) {
    fill_dp(-1);

    int moves = 0;

    
    for(int i = 0; i < board.size(); i++) {
      if(board[i] == 'o') {
	moves += board.size() - 1 - i;
      }
    }

    if(moves % 2 == 0) {
      return "NO"; 
    } else {
      return "YES";
    }

  }
private : 

#if 0
  int board_to_int(string board) {
    int ret_val = 0;
    for(int i = 0; i < board.size(); i++) {
      if(board[i] == 'o') {
	ret_val |= (1 << ((board.size() - 1) - i));
      }
    }
    return ret_val;
  }

  void rec(string board, bool first_player) {
    if(board[board.size() - 1] == 'o') {
      board[board.size() - 1] = '.';
    }

    if(dp[board_to_int] == -1) {
      /* Check possible options */
      vector<string> next_move;

      for(int i = 0; i < board.size() - 1; i++) {
        if(board[i] == 'o' && board[i + 1] == '.') {
          string new_board = board;
          new_board[i] = '.';
          new_board[i + 1] = 'o';
          next_move.push_back(new_board);
        }

        if(i + 3 < board.size()) {
          if(board[i] == 'o' && board[i + 1] == 'o' && board[i + 2] == 'o' && board[i + 3] == '.') {
            string new_board = board;
            new_board[i] = '.';
            new_board[i + 3] = 'o';
            next_move.push_back(new_board);
          }
        }
      }

      for(int i = 0; i < next_move.size(); i++) {
        rec(next_move[i]);
      }

    } else {

    }

  }
#endif
};
