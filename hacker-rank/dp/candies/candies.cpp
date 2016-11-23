#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

#define INC 1
#define DEC 2
#define MONO 3
#define EQUAL 4

vector<int> rating;
vector<int> sequence;
vector<vector<int> > inc_seq;
vector<vector<int> > dec_seq;
vector<vector<int> > equal_seq;

int get_seq(int a, int b) {
  if(a == b) { return EQUAL; }
  if(a < b) { return INC; }
  if(a > b) { return DEC;} 
}

void print_vector(vector<vector<int> > seq, int idx) {
  for(int i = 0; i < seq[idx].size(); i++) {
    cout<<seq[idx][i]<<" ";
  }
}

void gen_seq(vector<int> rating) {

  int start_pos = 0;
  while(1) {

    if(start_pos >= rating.size() - 1) {
      break;
    }

    vector<int> seq; 

    int set_seq = 0;
    int i = start_pos;
    for(i = start_pos; i < rating.size() - 1; i++) {
      if(i == start_pos) {
	if(rating[i] == rating[i + 1]) { 
	  set_seq = EQUAL; 
	} else if(rating[i] < rating[i + 1]) {
	  set_seq = INC;
	} else if(rating[i] > rating[i + 1]) {
	  set_seq = DEC;
	}

	seq.push_back(rating[i]);
	seq.push_back(rating[i + 1]);
      } else {
	if(set_seq != get_seq(rating[i], rating[i + 1])) {
	  start_pos = i + 1;
	  break;
	} else {
	  seq.push_back(rating[i + 1]);
	}
      }

    }

    start_pos = i + 1;

    if(set_seq == INC) {
      inc_seq.push_back(seq);
      sequence.push_back(INC);
    } else if(set_seq == DEC) {
      dec_seq.push_back(seq);
      sequence.push_back(DEC);
    } else if(set_seq == EQUAL) {
      equal_seq.push_back(seq);
      sequence.push_back(EQUAL);
    }

  }

  if(start_pos == rating.size() - 1) { sequence.push_back(MONO); }  

}

int process_inc(vector<int> inc) {
  return ((inc.size() * (inc.size() + 1)) / 2);
}
int process_dec(vector<int> dec) {
  return ((dec.size() * (dec.size() + 1)) / 2);
}
int process_equal(vector<int> equal) {
  return equal.size();
}
int process_sequence(vector<int> sequence) {
  assert(sequence.size() > 0);

  if(sequence[0] == MONO) { return 1; }
  
  int inc_idx = 0;
  int dec_idx = 0;
  int eql_idx = 0;

  int base_val = 0;

  if(sequence[0] == INC) { 
    base_val = process_inc(inc_seq[inc_idx]); inc_idx++;
  } else if(sequence[0] == DEC) {
    base_val = process_dec(dec_seq[dec_idx]); dec_idx++;
  } else if(sequence[0] == EQUAL) {
    base_val = process_equal(equal_seq[eql_idx]); eql_idx++; 
  }

  for(int i = 1; i < sequence.size(); i++) {
    switch(sequence[i]) {
      case INC :
		  if(sequence[i - 1] == INC) {
		    base_val += process_inc(inc_seq[inc_idx]); inc_idx++;
		  }
      case DEC :
      case EQUAL :
      case MONO :
      default :
	assert(0);
    }
  }
}

int main() {

  int T;
  cin>>T;

  while(T--) {

    int N;
    cin>>N;

    rating.clear();
    sequence.clear();
    inc_seq.clear();
    dec_seq.clear();
    equal_seq.clear();

    for(int i = 0; i < N; i++) {
      int element;
      cin>>element;
      rating.push_back(element);
    }

    gen_seq(rating);

    cout<<process_sequence(sequence)<<endl;

#if 0

    int inc_idx = 0;
    int dec_idx = 0;
    int eql_idx = 0;

    cout<<"+++++++++++++++++++++++++++++++++++++++++"<<endl;
    for(int i = 0; i < rating.size(); i++) {
      cout<<rating[i]<<" ";
    }
    cout<<endl;

    for(int i = 0; i < sequence.size(); i++) {
      int curr_seq = sequence[i];
      if(curr_seq == INC) {
        print_vector(inc_seq, inc_idx);
	inc_idx++;
      } else if(curr_seq == DEC) {
        print_vector(dec_seq, dec_idx);
	dec_idx++;
      } else if(curr_seq == EQUAL) {
        print_vector(equal_seq, eql_idx);
	eql_idx++;
      } else if(curr_seq == MONO) {
        cout<<rating[rating.size() - 1]<<endl;
        assert(i == sequence.size() - 1);
      }
    }
    cout<<endl;
    cout<<"+++++++++++++++++++++++++++++++++++++++++"<<endl;
#endif

  }
  return 0;
}
