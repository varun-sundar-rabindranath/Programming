#include <iostream>
#include <vector>
using namespace std;

class BadNeighbors {
public : 
      int maxDonations(vector<int> donations) {
	if(donations.size() == 1) { return donations[0]; }
	if(donations.size() == 2) { return max(donations[0], donations[1]); }
	if(donations.size() == 3) { return max(max(donations[0], donations[1]), donations[2]); }

	donations.push_back(donations[0]);

	int num_elements = donations.size();
	int last_index = num_elements - 1;
	int last_house_donation = donations[0];

	vector<int> consider, not_consider;

	consider.push_back(donations[0]);
	not_consider.push_back(0);

	for(int i = 1; i < donations.size(); i++) {
	  int donation = donations[i];
	  int p_c = consider[i - 1];
	  int p_nc = not_consider[i - 1];

	  int curr_nc = p_c;
	  int curr_c = 0;

	  if(i > 2) {
	    curr_c = max(p_nc + donation, donation + consider[i - 3]);
	  } else {
	    curr_c = p_nc + donation;
	  }

	  consider.push_back(curr_c);
	  not_consider.push_back(curr_nc);
	}

#if 0
	//cout<<"Not consider : ";
	for(int i = 0; i < not_consider.size(); i++) {
	  cout<<not_consider[i]<<" ";
	}
	cout<<endl;
	//cout<<"Consider : ";
	for(int i = 0 ; i < consider.size(); i++) {
	  cout<<consider[i]<<" ";
	}
	cout<<endl;

#endif

	/* Check if the last element won */
	if (consider[last_index] >= not_consider[last_index]) {
	  if(consider[last_index] == consider[last_index - 3] + last_house_donation) {
	    return consider[last_index - 3];
	  } else {
	    return not_consider[last_index - 1];
	  }
	} else {
	  return consider[last_index - 1] - last_house_donation;
	}
      }
};

int main() {
  BadNeighbors b;

  int T;
  cin>>T;

  while(T--) {
    int K;
    cin>>K;

    vector<int> donations;

    for(int i = 0; i < K; i++) {
      int e;
      cin>>e;
      donations.push_back(e);
    }
    cout<<b.maxDonations(donations)<<endl;
  }

  return 0;
}
