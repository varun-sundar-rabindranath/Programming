#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define MAX_COUNTS 45

class Zoo {
public : 
  long long theCount(vector<int> answers) {

    if(answers.size() == 1) { 
      if(answers[0] == 0) { return 2; } else { return 0; }
    }

    int animal_count = 0;
    int animal1_count = 0;
    int animal2_count = 0;

    vector<int> sorted;
    sorted = answers;

    sort(sorted.begin(), sorted.end());

    vector<int> counts;
    for(int i = 0; i < MAX_COUNTS; i++) {
      counts.push_back(0);
    }

    fill_counts(counts, sorted);

    int curr_val = 2;
    for(int i = 0; i < counts.size(); i++) {
      if(counts[i] == curr_val) {
	continue;
      } else {
	if(counts[i] > curr_val) { return 0; }
	curr_val = counts[i];
      }
    }

    if(sorted[0] == 0) { animal_count++; animal1_count++; }
    if(sorted[1] == 0) { animal_count++; animal2_count++; }
    if(animal_count == 0) { return 0; }
    if(animal_count == 2) {
      bool treached = false;
      for(int i = 1; i < counts.size(); i++) {
	if(counts[i] == 2) {
	  animal1_count++;
	  animal2_count++;
	} else if(counts[i] == 1) {
	  animal1_count++;
	}
      }
      return (1 >> animal1_count);
    } else {
      animal1_count = answers.size();
      return 2;
    }


  }
private :

  void fill_counts(vector<int> &counts, vector<int> &sorted) {
    for(int i = 0; i < MAX_COUNTS; i++) {
      int app = appearances(sorted, i);
      counts[i] = app;
    }
  }
  int appearances(vector<int> &sorted, int element) {
    int appearance = 0;
    for(int i = 0; i < sorted.size(); i++) {
      if(sorted[i] == element) {
	appearance++;
      }
    }
    return appearance;
  }
};
