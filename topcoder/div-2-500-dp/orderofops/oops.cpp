#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <climits>
using namespace std;

class OrderOfOperationsDiv2 {
public :

  void print_vector(vector<string> &order) {
    for(int i = 0; i < order.size(); i++) {
      cout<<order[i]<<endl;
    }
  }

  int order_cost(vector<string> &order) {
    int cost = 0;
    string in_cache = "00000000000000000000";
    for(int i = 0; i < order.size(); i++) {
      int newly_seen = 0;
      for(int j = 0; j < order[i].size(); j++) {
	if(order[i][j] == '1' && in_cache[j] == '0') {
	  newly_seen++;
	  in_cache[j] = 1;
	}
      }
      //cout<<"Newly seend and power -> "<<newly_seen<<"  "<<pow(newly_seen, 2)<<endl;
      cost += (int)pow(newly_seen, 2);
    }

    print_vector(order);
    cout<<" incurred cost --> "<<cost<<endl;

    return cost;
  }

  void add_to_order(vector<string> &min_order, string s) {
    int pos = 0;
    vector<string>::iterator it;

    int min_cost = INT_MAX;

    vector<string> change_order = min_order;

    for(int i = 0; i <= min_order.size(); i++){
      /* Insert str at this pos */

      change_order.insert(change_order.begin() + i, s);
      int this_cost = order_cost(change_order);
      change_order.erase(change_order.begin() + i);

      if(this_cost < min_cost) {
	min_cost = this_cost;
	pos = i;
      }
    }

    min_order.insert(min_order.begin() + pos, s);
  }

  int minTime(vector<string> s) {


    vector<string> min_order;

    for(int i = 0; i < s.size(); i++) {
      //cout<<"--- NI ---"<<endl;
      //print_vector(min_order);
      cout<<"-----"<<order_cost(min_order)<<"----"<<endl;
      add_to_order(min_order, s[i]);
    }

    return order_cost(min_order);
  }
};

int main() {
  OrderOfOperationsDiv2 oops;
  
 // char arr_ip[20][30] = {
 //   "00001001001000000", 
 //   "00000000000000000", 
 //   "01001001000000100", 
 //   "01000000010000000", 
 //   "00000010011000000", 
 //   "00100000000000100", 
 //   "00100000000000000", 
 //   "01000011000000000", 
 //   "10000000000000000", 
 //   "00000101000000000", 
 //   "00100000000010000", 
 //   "00010010000000010", 
 //   "00000000000000000", 
 //   "00100000000000000", 
 //   "00000010000000010", 
 //   "00000000000100000", 
 //   "10000000000010000", 
 //   "00010000000000001", 
 //   "00100000000100000", 
 //   "00001000000000000"};

 char arr_ip[4][20] = {
 "11000000",
 "00110000",
 "00001111",
 "10101111"};

  vector<string> ip;
  for(int i = 0; i < 4; i++) {
    ip.push_back(arr_ip[i]);
  }

  cout<<oops.minTime(ip)<<endl;

  return 0;

}
