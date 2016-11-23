/* Using SPOJ's ADVEDIST problem to test the correctness of the impl */
#include <iostream>
#include <string>
#include <climits>
#include <cstdlib>
#include <cstdio>
using namespace std;

#define STRLEN 1005

short dlev[STRLEN][STRLEN];

void fill_zero() {
  for(int i = 0; i < STRLEN; i++) {
    for(int j = 0; j < STRLEN; j++) {
      dlev[i][j] = 0;
    }
  }
}

short damereau_levenshtein(string a, string b, short dp[STRLEN][STRLEN]) {
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
      /* Need to look at 4 possibilities */

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
      /* Swap cost; Consider the 'a' and 'b' substrings until 'j - 3' and 'i - 3';
       * Determine what would be the cost if swapping the current char and the previous
       * char is possible;
       */
      short swap_cost = SHRT_MAX;
      if(i - 2 >= 0 && j - 2 >= 0) {
	if((a[j - 1] == b[i - 2] && a[j - 2] == b[i - 1]))
	  swap_cost = dp[i - 2][j - 2] + 1;
      }

      dp[i][j] = min(min(substituition_cost, swap_cost), min(deletion_cost, insertion_cost));

    }
  }

  cost = dp[b.size()][a.size()];

  return cost;
}

/* Wont be used in spell check !! */
short damereau_levenshtein_transposition(string a, string b, short dp[STRLEN][STRLEN]) {
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

  bool *used = NULL;
  used = (bool *)calloc(1, a.size());

  for(int i = 1; i < b.size() + 1; i++) {
  
    for(int k = 0; k < a.size(); k++) {
      used[k] = false;
    }

    for(int j = 1; j < a.size() + 1; j++) {
      /* Need to look at 4 possibilities */

      /* Lets look at the min cost of substrings without considering
       * the current characters (That is strings 'a' and 'b' untill chars
       * 'i - 2' and 'j - 2' resp.); Now if we sustitute the current 
       * char in 'a' with the current char in b what would be the cost ???????
       */
      short sub_cost = (a[j - 1] != b[i - 1]);
      short substituition_cost = dp[i - 1][j - 1] + sub_cost;
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
      /* Swap cost; Consider the 'a' and 'b' substrings until 'j - 3' and 'i - 3';
       * Determine what would be the cost if swapping the current char and the previous
       * char is possible;
       */
      short swap_cost = SHRT_MAX;
      if(i - 2 >= 0 && j - 2 >= 0) {
	if(b[i - 2] == a[j - 1]) {
	  bool swap_present = false;
	  int k = j - 1;
	  while(k >= 0) {
	    if(used[k]) { break; }
	    if(a[k] == b[i - 1]) {
	      swap_present = true;
	      break;
	    }
	    k--;
	  }
	  
	  if(swap_present) { swap_cost = dp[i - 1][j - 1]; }
	}
      }

      dp[i][j] = min(min(substituition_cost, swap_cost), min(deletion_cost, insertion_cost));
      
      if(dp[i][j] == substituition_cost) { used[j - 1] = true; }
      if(dp[i][j] == swap_cost) {used[j - 1] = true; }

    }
  }

  free(used);

  cost = dp[b.size()][a.size()];

  return cost;
} 

int main() {

  string a, b;

  cin>>a;
  cin>>b;

  while(a!="*" && b!="*") {
    cout<<damereau_levenshtein(a, b, dlev)<<endl;
    cin>>a;
    cin>>b;
  }

  return 0;

}
