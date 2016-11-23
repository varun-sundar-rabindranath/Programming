/* Using SPOJ's EDIST problem to test the correctness of the impl */
#include <iostream>
#include <string>
using namespace std;

#define STRLEN 2005

short lev[STRLEN][STRLEN];

void fill_zero() {
  for(int i = 0; i < STRLEN; i++) {
    for(int j = 0; j < STRLEN; j++) {
      lev[i][j] = 0;
    }
  }
}

short levenshtein(string a, string b, short dp[STRLEN][STRLEN]) {
  short cost = 0;

  /* Initialize lev */
  fill_zero();

  /* Initialize row and column corresponding to null string */
  for(int i = 1; i < a.size() + 1; i++) {
    dp[0][i] = i;
  }
  for(int i = 1; i < b.size() + 1; i++) {
    dp[i][0] = i;
  }

  for(int i = 1; i < b.size() + 1; i++) {
    for(int j = 1; j < a.size() + 1; j++) {
      /* Need to look at 3 possibilities */

      /* Lets look at the min cost of substrings without considering
       * the current characters (That is strings 'a' and 'b' untill chars
       * 'i - 2' and 'j - 2' resp.); Now if we sustitute the current 
       * char in 'a' with the current char in b what would be the cost ???????
       */
      short substituition_cost = dp[i - 1][j - 1] + (a[j - 1] != b[i - 1]);
      /* Lets look at the min cost of substrings 'a' until 'j - 1' and
       * 'b' until 'i - 2'; Now if we are to insert 'b[i - 1]' to the end of
       * that min cost, that would be out cost for insertion
       */
      short insertion_cost = dp[i - 1][j] + 1;
      /* Deletion cost ; We dont need the current a[j - 1]; Just delete it;
       * In this case it means that we are satisfied with the edit distance
       * got from without cosidering a[j - 1]; However we need to add the
       * deletion cost
       */
      short deletion_cost = dp[i][j - 1] + 1;

      dp[i][j] = min(substituition_cost, min(deletion_cost, insertion_cost));

    }
  }

  cost = dp[b.size()][a.size()];

  return cost;
}

int main() {

  int T;
  cin>>T;

  while(T--) {

    string a;
    string b;
    cin>>a;
    cin>>b;

    cout<<levenshtein(a, b, lev)<<endl;

  }

  return 0;

}
