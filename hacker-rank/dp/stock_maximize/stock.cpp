#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

vector<int> stock_prices;
vector<int> sell_index;

int main() {
  int T;
  cin>>T;

  while(T--) {
    int N;
    cin>>N;

    stock_prices.clear();
    sell_index.clear();

    for(int i = 0; i < N; i++) {
      int price;
      cin>>price;
      stock_prices.push_back(price);
    }

    int start_pos = 0;
    while(start_pos < stock_prices.size()) {
      int max_val = -1;
      int max_index = -1;
      for(int i = start_pos; i < stock_prices.size(); i++) {
	if(stock_prices[i] > max_val) {
	  max_val = stock_prices[i];
	  max_index = i;
	}
      }	

      assert(max_index != -1);

      sell_index.push_back(max_index);

      start_pos = max_index + 1;
    }

    /* Simulate buying and selling */
    long long int profit = 0;
    start_pos = 0;
    for(int i = 0; i < sell_index.size(); i++) {
      int sell_price = stock_prices[sell_index[i]];
      for(int j = start_pos; j < sell_index[i]; j++) {
	profit += sell_price - stock_prices[j];
      }	
      start_pos = sell_index[i] + 1;
    }

    cout<<profit<<endl;


  }
  return 0;
}
