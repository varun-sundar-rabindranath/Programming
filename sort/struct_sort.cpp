#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

typedef struct sort_dt {

  std::string ts;

  float ts_float;

  sort_dt(std::string ts_, double ts_float_) {
    ts       = ts_;
    ts_float = ts_float_;
  }

}sort_dt;

static bool sort_dt_compare(sort_dt s1, sort_dt s2) {

  if (s1.ts < s2.ts) { return true;  }
  else               { return false; }
}

void print_sort_dt (std::vector<sort_dt> sort_dt_vec) {

  std::vector<sort_dt>::iterator sdt;

  for (sdt = sort_dt_vec.begin(); sdt != sort_dt_vec.end(); sdt++) {
    std::cout<<sdt->ts<<" "<<sdt->ts_float<<std::endl;
  }
}

int main() {

  std::vector<sort_dt> sort_dt_vec;

  sort_dt_vec.push_back(sort_dt("0-00-40.000", 40));
  sort_dt_vec.push_back(sort_dt("0-00-01.000", 1));
  sort_dt_vec.push_back(sort_dt("0-00-00.000", 0));
  sort_dt_vec.push_back(sort_dt("0-01-00.000", 60));
  sort_dt_vec.push_back(sort_dt("0-00-00.234", 0.234));
  sort_dt_vec.push_back(sort_dt("0-01-40.001", 100.001));
  sort_dt_vec.push_back(sort_dt("0-01-40.000", 100));
  sort_dt_vec.push_back(sort_dt("1-00-00.231", 3600.231));
  sort_dt_vec.push_back(sort_dt("0-02-00.000", 120));
  sort_dt_vec.push_back(sort_dt("0-00-35.000", 35));
  sort_dt_vec.push_back(sort_dt("1-00-00.000", 3600));
  sort_dt_vec.push_back(sort_dt("0-10-00.000", 600));

  sort(sort_dt_vec.begin(), sort_dt_vec.end(), sort_dt_compare);

  print_sort_dt(sort_dt_vec);

  return 0;
}
