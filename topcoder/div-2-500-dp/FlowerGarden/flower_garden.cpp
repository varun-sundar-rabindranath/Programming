#include <iostream>
#include <vector> 
#include <utility>
#include <cassert>
#include <algorithm>
#include <climits>

using namespace std;

class FlowerGarden {
public :

  vector<int> check_overlap(vector< pair<int, int> > &extent, int b, int w) {

    vector<int> ret_vector;

    for(int i = 0; i < extent.size(); i++) {
      int c_bloom = extent[i].first;
      int c_wilt = extent[i].second;

      if(b > c_wilt || w < c_bloom) {
	//no overlap;
	continue;
      } else {

//	cout<<b<<", "<<w<<" overlaps with "<<c_bloom<<", "<<c_wilt<<endl;

	ret_vector.push_back(i);
      }
    }
    
#if 0
    /* Print ret vec */
    cout<<" Print retvector \n ";
    for(int i = 0; i < ret_vector.size(); i++) {
      cout<<ret_vector[i]<<" ";
    }
    cout<<endl;
#endif
    return ret_vector;
  }

  void merge_overlap_vectors(vector<int> &overlap, vector<pair<int, int> > &extent, vector<vector<int> > &order_height) {
    vector<vector<int> > new_order_height;
    vector<pair<int, int> > new_extent;

    vector<int> merged_order_height;
    pair<int, int> merged_extent(INT_MAX, -1);

    assert(extent.size() == order_height.size());

    for(int i = 0; i < order_height.size(); i++) {
      bool to_merge = false;
      for(int j = 0; j < overlap.size(); j++) {
	if(i == overlap[j]) { to_merge = true; break; }	
      }

      if(to_merge) {
	merged_order_height.insert(merged_order_height.begin(), order_height[i].begin(), order_height[i].end());
	sort(merged_order_height.begin(), merged_order_height.end());

	merged_extent.first = min(merged_extent.first, extent[i].first);
	merged_extent.second = max(merged_extent.second, extent[i].second);
      } else {
	new_order_height.push_back(order_height[i]);
	new_extent.push_back(extent[i]);
      }
    }

    new_order_height.push_back(merged_order_height);
    new_extent.push_back(merged_extent);

    extent.clear();
    order_height.clear();

    extent.insert(extent.begin(), new_extent.begin(), new_extent.end());
    order_height.insert(order_height.begin(), new_order_height.begin(), new_order_height.end());

    //extent = new_extent;
    //order_height = new_order_height;
  }
  
  vector<int> getOrdering(vector<int> height, vector<int> bloom, vector<int> wilt) {
    vector< vector<int> > order_height;
    vector< pair<int, int> > extent;

    for(int i = 0; i < height.size(); i++) {
      int h = height[i];
      int b = bloom[i];
      int w = wilt[i];

      vector<int> overlap = check_overlap(extent, b, w);

      if(overlap.size() == 0) {
	/* No overlap; hence add to extent */
	extent.push_back(pair<int, int>(b, w));
	vector<int> new_vec;
	new_vec.push_back(h);
	order_height.push_back(new_vec);
      } else {
	vector<int> new_vec;
	new_vec.push_back(h);
	order_height.push_back(new_vec);
	extent.push_back(pair<int, int> (b, w));
	overlap.push_back(order_height.size() - 1);
	merge_overlap_vectors(overlap, extent, order_height);
#if 0
    /* Print order height */
    cout<<" Print order height \n";
    for(int i = 0; i < order_height.size(); i++) {
      for(int j = 0; j < order_height[i].size(); j++) {
	cout<<order_height[i][j]<<" ";
      }
      cout<<" : Extent : ";
      cout<<extent[i].first<<", "<<extent[i].second;
      cout<<endl;
    }
#endif
      }
    }


    vector<pair<int, int> > to_assoc_sort;
    for(int i = 0; i < order_height.size(); i++) {
      to_assoc_sort.push_back(pair<int, int>(order_height[i][order_height[i].size() - 1], i));
    }

    sort(to_assoc_sort.begin(), to_assoc_sort.end());

    vector<int> ret_vec;

    for(int i = to_assoc_sort.size() - 1; i >= 0; i--) {
      int index = to_assoc_sort[i].second;
      for(int j = 0; j < order_height[index].size(); j++) {
	int val = order_height[index][j];
	ret_vec.push_back(val);
      }	
    }

    return ret_vec;
  }
};

int main() {
  FlowerGarden fg;

  vector<int> height;
  vector<int> bloom;
  vector<int> wilt;

  for(int i = 1; i <= 6; i++) {
    height.push_back(i);
    if(i%2 == 0) {
      bloom.push_back(3);
      wilt.push_back(4);
    } else {
      bloom.push_back(1);
      wilt.push_back(2);
    }

  }

  vector<int> ret_val = fg.getOrdering(height, bloom, wilt);

  for(int i = 0; i < ret_val.size(); i++) {
    cout<<ret_val[i]<<" ";
  }

  return 0;
}

