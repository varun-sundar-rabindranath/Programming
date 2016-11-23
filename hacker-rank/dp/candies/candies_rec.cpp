#include <iostream>
#include <vector>
using namespace std;

vector<int> rating;
vector<int> num_choc;
int N;

int rec(int idx, int height) {
  if(idx == 0) { num_choc[idx] = max(height + 1, 1); return (max(height + 1, 1)); }

  if(rating[idx] == rating[idx - 1]) {
    rec(idx - 1, 0);
    num_choc[idx] = max(height + 1, 1);
    return (max(height + 1, 1));
  }

  if(rating[idx] < rating[idx - 1]) {
    num_choc[idx] = min( height + 1, rec(idx - 1, height + 1) - 1);
    return num_choc[idx];
  }

  if(rating[idx] > rating[idx - 1]) {
    num_choc[idx] = max(height + 1, rec(idx - 1, 0) + 1);
    return num_choc[idx];
  }

  return 0;
}

int main() {

  int T;
  cin>>T;

  while(T--) {

    num_choc.clear();
    rating.clear();

    cin>>N;

    num_choc.resize(N);
     
    for(int i = 0; i < N; i++) {
      int element;
      cin>>element;
      rating.push_back(element);
    }

    rec(rating.size() - 1, 0);

    int sum = 0;
    /* Sum num_choc */
    for(int i = 0; i < num_choc.size(); i++) {
      sum += num_choc[i];
    }


    cout<<sum<<endl;

#  if 0
    cout<<"-----------------------------------"<<endl;
    for(int i = 0; i < rating.size(); i++) {
      cout<<rating[i]<<" ";
    }
    cout<<endl;
    for(int i = 0; i < num_choc.size(); i++) {
      cout<<num_choc[i]<<" ";
    }
    cout<<endl<<"-----------------------------------"<<endl;
#endif

  }

  return 0;
}
