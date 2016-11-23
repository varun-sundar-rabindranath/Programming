

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

typedef std::pair<bool, std::string> my_pair;

bool sort_pred(const my_pair& left, const my_pair& right)
    {
    return left.second < right.second;
    }

int main()
    {
    std::vector<my_pair> data;

    data.push_back(my_pair(false, "orange"));
    data.push_back(my_pair(true, "apple"));
    data.push_back(my_pair(true, "pear"));

    std::sort(data.begin(), data.end(), sort_pred);
    }
