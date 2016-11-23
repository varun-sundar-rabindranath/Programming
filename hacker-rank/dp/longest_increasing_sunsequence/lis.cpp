#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

#define MAX_N 1000007

void process_lis_list(vector<pair< int, int > > &lis_list, int element) {
  vector<pair<int, int> >::iterator it;

  pair<int, int> ins_pair;
  ins_pair.first = element;
  ins_pair.second = -1;

  pair<int, int> existing_pair;
  existing_pair.first = -1;
  existing_pair.second = -1;

  vector< vector<pair<int, int> >::iterator > del;
  vector<pair<int, int> >::iterator del_candidate;

  for(it = lis_list.begin(); it != lis_list.end(); it++){
    if(it->first == element) {
      existing_pair.first = it->first;
      existing_pair.second = it->second;
      del.push_back(it);
    }

    if(it->first < element) {
      if(it->second > ins_pair.second) { 
	ins_pair.second = it->second;
	del_candidate = it;	
      } 
    }
  }

  for(int i = 0; i < del.size(); i++) {
    lis_list.erase(del[i]);
  }

  if(ins_pair.second != -1) { 
    ins_pair.second += 1; 
    lis_list.erase(del_candidate);
  }

  if(existing_pair.second != -1) {
    /* Pair exists */
    if(ins_pair.second != -1) {
      /* No new pair */
      /* Insert existing pair */
      lis_list.push_back(existing_pair);
    } else {
      /* New pair also exists */
      if(ins_pair.second > existing_pair.second) {
	lis_list.push_back(ins_pair);
      } else {
	lis_list.push_back(existing_pair);
      }
    }
  } else {
    /* No existing pair */
    if(ins_pair.second == -1) {
      /* New list */
      ins_pair.second = 1;
      lis_list.push_back(ins_pair);
    } else {
      lis_list.push_back(ins_pair);
    }
  }
}

void delete_greater(vector<pair<int, int > > &lis_list, vector<vector<pair<int, int > >::iterator > &greater, int e, int val) {

  vector<pair<int, int > >::iterator it;
  for(it = lis_list.begin(); it != lis_list.end(); it++) {
    if(it->first > e && it->second <= val) {
      lis_list.erase(it);
      it--;
    }
  }
#if 0
  for(int i = 0; i < greater.size(); i++) {
    if(greater[i]->second <= val) {
      lis_list.erase(greater[i]);
    }
  }
#endif
}

void process_lis_list_again(vector<pair<int, int> > &lis_list, int element) {
  pair<int, int> new_pair;
  new_pair.first = element;
  new_pair.second = -1;

  pair<int, int> existing_pair;
  existing_pair.first = element;
  existing_pair.second = -1;

  vector< vector<pair<int, int> >::iterator > greater;
  vector< vector<pair<int, int> >::iterator > smaller;

  vector<pair<int, int > >::iterator lis_it;
  vector<pair<int, int > >::iterator del1;
  vector<pair<int, int > >::iterator del2;

  for(lis_it = lis_list.begin(); lis_it != lis_list.end(); lis_it++) {
    if(lis_it->first < element) {
      if(lis_it->second > new_pair.second) {
	new_pair.second = lis_it->second;
	del1 = lis_it;
      }
    }

    if(lis_it->first > element) {
      greater.push_back(lis_it);
    }
    
    if(lis_it->first == element) {
      existing_pair.second = lis_it->second;
      del2 = lis_it;
    }
  }

  if(new_pair.second != -1) { new_pair.second += 1; }
  
  if(existing_pair.second != -1) {
    /* pair exists */
    if(new_pair.second != -1) {
      /* new pair also available */
      if(new_pair.second > existing_pair.second) {
	delete_greater(lis_list, greater, element, new_pair.second);
	lis_list.push_back(new_pair);
	lis_list.erase(del2);
	lis_list.erase(del1);
      }
    } 
  } else {
    if(new_pair.second == -1) {
      new_pair.second = 1;
      delete_greater(lis_list, greater, element, new_pair.second);
      lis_list.push_back(new_pair);
    } else {
      delete_greater(lis_list, greater, element, new_pair.second);
      lis_list.push_back(new_pair);
      lis_list.erase(del1);
    }
  }

}

int lis(vector<int> sequence) {
  vector<int> len;
  len.resize(sequence.size());

  vector<pair<int, int> > lis_list;


  for(int i = 0; i < sequence.size(); i++) {
    
    process_lis_list_again(lis_list, sequence[i]);  

#if 0
    int maxval = 0;

    for(int j = 0; j < i; j++) {
      if(sequence[j] < sequence[i]) {
	maxval = max(maxval, len[j]);
      }
    }

    len[i] = maxval + 1; 
#endif
  }

  int retval = 0;

  for(int i = 0; i < lis_list.size(); i++) {
    retval = max(retval, lis_list[i].second);
    cout<<lis_list[i].first<<endl;
  }
  
  return retval;
}

int main() {

  int N;

  cin>>N;

  vector<int> sequence;

  for(int i = 0; i < N; i++) {
    int val;
    cin>>val;
    sequence.push_back(val);
  }

  cout<<lis(sequence)<<endl;

  return 0;
}
