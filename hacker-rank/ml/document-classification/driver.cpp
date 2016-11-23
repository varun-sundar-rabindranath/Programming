#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cassert>
#include <cmath>
using namespace std;

#define ALPHABETS 26

/* Interface file for trie data structure */

/* Forward decl of trie structures */
typedef struct trie_node {
  trie_node * next_letter[ALPHABETS];
  int frequency;

  trie_node() {
    frequency = 0;
    for(int i = 0; i < ALPHABETS; i++) {
      next_letter[i] = NULL;
    }
  }
} trie_node;

typedef struct trie {
  trie_node *node;
  
  trie() {
    node = NULL;
  }
} trie;

/* Initialize the trie structure */
void init_trie(trie *t);

/* Free trie and trie_nodes associated with trie */
void destroy_trie(trie *t);

/* Add string str to trie */
void add_string(trie *t, string &str);

/* Print contents of the trie */
void print_trie(trie *t);

/* Allocate memory for a trie_node */
trie_node *create_trie_node();

/* Free node ptr */
void free_trie_node(trie_node *node);

/* Print trie_node's data; s is the string that led to that trie_node */
void print_trie_node_data(trie_node *node, string s);

/* Traverse trie and get frequencies of the strings */
vector<pair<string, int> > get_word_frequency(trie *t);

/* Get a given strings occurance count */
int get_str_count(trie *t, string str);

/* Trie implementation */

/* Initialize the trie structure */
void init_trie(trie *t) {
  t->node = create_trie_node();
}

/* Add string str to trie */
void add_string(trie *t, string &str) {


  trie_node *node = t->node;

  for(int i = 0; i < str.size(); i++) {
    char c = str[i];
    int alpha = c - 97;
    if(node->next_letter[alpha] == NULL) {
      node->next_letter[alpha] = create_trie_node(); 
    } 
    node = node->next_letter[alpha];
  }
  node->frequency++;
}  

void print_trie_recursively(trie_node *node, string s) {
  if(node->frequency != 0) {
    print_trie_node_data(node, s);
  }
  for(int i = 0; i < ALPHABETS; i++) {
    if(node->next_letter[i] != NULL) {
      char c[2];
      c[0] = i + 97;
      c[1] = '\0';
      print_trie_recursively(node->next_letter[i], s + string(c));
    }
  }
}

/* Print contents of the trie */
void print_trie(trie *t) {
  print_trie_recursively(t->node, "");
}

/* Print trie_node's data; s is the string that led to that node */
void print_trie_node_data(trie_node *node, string s) {
  cout<<s<<" --> "<<node->frequency<<endl;
}

/* Allocate memory for a trie_node */
trie_node *create_trie_node() {
  trie_node *node;
  node = (trie_node *)calloc(1, sizeof(trie_node));
  return node;
}

/* Free node ptr */
void free_trie_node(trie_node *node) {
  free(node);
}

void destroy_trie_recursively(trie_node *node) {
  for(int i = 0; i < ALPHABETS; i++) {
    if(node->next_letter[i] != NULL) {
      destroy_trie_recursively(node->next_letter[i]);
    }
  }
  free_trie_node(node);
}

/* Free trie and trie_nodes associated with trie */
void destroy_trie(trie *t) {
  /* Destroy all the trie nodes associated */
  destroy_trie_recursively(t->node);
}

void get_word_frequency_recursively(trie_node *node, string s, vector<pair<string, int> > &vec) {
  if(node->frequency != 0) {
    vec.push_back(pair<string, int> (s, node->frequency));
  }
  for(int i = 0; i < ALPHABETS; i++) {
    if(node->next_letter[i] != NULL) {
      char c[2];
      c[0] = i + 97;
      c[1] = '\0';
      get_word_frequency_recursively(node->next_letter[i], s + string(c), vec);
    }
  }
}

/* Traverse trie and get frequencies of the strings */
vector<pair<string, int> > get_word_frequency(trie *t) {
  vector<pair<string, int> > ret_vector;
  get_word_frequency_recursively(t->node, "", ret_vector);
  return ret_vector;
}

/* Get a given strings occurance count */
int get_str_count(trie *t, string str) {
  trie_node *node = t->node;

  bool str_present = true;
  
  for(int i = 0; i < str.size(); i++) {
    char c = str[i];
    int alpha = c - 97;
    if(node->next_letter[alpha] == NULL) {
      str_present = false;
      break;
    } 
    node = node->next_letter[alpha];
  }

  if(str_present) {
    return node->frequency;
  } else {
    return 0;
  }
}

/*---------------------------- Above is the trie impl --------------------------- */

#define CLASS 8
#define F 24

float theta_arr[CLASS][F + 1] = {
-0.399258,0.0574898,0.945368,0.274595,0.67528,0.941802,0.685561,0.522402,0.855703,0.225106,-0.597971,0.340478,0.337905,0.0618339,0.537298,-0.071576,0.612335,-0.225795,0.886149,0.50578,0.919036,0.917136,-0.0487296,-0.320135,0.995797,
-0.319258,0.0798525,0.92685,0.315313,0.671125,-0.220143,0.605534,0.643232,0.856585,0.624101,-0.0542956,0.350457,0.349857,-0.412578,0.563032,-0.105878,0.614428,-0.193292,0.889331,0.368956,0.912355,0.909744,-0.133218,-0.255999,-0.413994,
-0.637536,0.110404,0.942092,-0.022914,0.744067,-0.0274725,0.713604,0.596857,0.884768,0.170769,-0.853809,0.782941,0.470274,-0.85648,0.643106,0.0542183,0.680428,-0.426652,0.908437,0.551232,0.923904,0.92583,0.88498,0.195248,-0.298764,
-0.647875,0.118518,0.942383,0.030692,0.752849,-0.0157951,0.695299,0.608417,0.891643,0.208242,-0.905277,0.443465,0.424399,-0.849158,0.594714,0.0983556,0.690221,-0.371061,0.987849,0.555149,0.995867,0.992032,0.0548889,-0.188128,-0.292866,
-0.686149,0.123795,0.976309,0.028906,0.755159,-0.0117368,0.70058,0.606972,1.00038,0.211522,-0.906511,0.43839,0.438848,-0.859534,0.599354,0.0995869,0.694743,-0.37851,0.912857,0.562726,0.931256,0.928454,0.0710902,-0.20205,-0.288972,
-0.609972,0.120394,0.943405,0.0449657,0.992368,0.010674,0.704682,0.603102,0.888279,0.202285,-0.824105,0.433725,0.498359,-0.713561,0.610683,0.873864,1.00362,-0.345221,0.926872,0.570669,0.930293,0.933391,0.0529276,-0.157492,-0.311921,
-0.645663,0.283252,0.948174,-0.0121916,0.752312,-0.024065,0.707049,0.59226,0.889562,0.188374,-0.906437,0.436626,0.486426,-0.919227,0.574066,0.0807347,0.69082,-0.160074,0.911779,0.824667,0.929094,0.927096,0.0497724,-0.196173,-0.296887,
-0.631932,0.291313,0.94759,-0.0137735,0.74624,-0.0255928,0.897778,0.773247,0.884058,0.189803,-0.88427,0.476084,0.605884,-0.777717,0.599067,0.0709553,0.686389,-0.378866,0.909749,0.622317,0.928248,0.92637,0.0552687,-0.110078,-0.302056};

char feature_words_arr[F][100] = {"bank", "certificates", "company", "crude", "cts", "dollar", "exchange", "grain", "inc", "it", "japan", "market", "mln", "more", "oil", "opec", "pct", "port", "rate", "shipping", "strike", "trade", "u", "vs" }; 
#if 0
float theta_arr[CLASS][41] = { -0.605286,0.892162,-0.116497,0.5572,0.635054,0.933067,0.276905,0.295516,0.650875,0.622001,0.966004,0.752371,0.0788736,0.624025,0.439409,0.830423,0.218793,-0.794732,0.235618,0.507674,0.19876,-0.0745532,0.479311,1.02614,0.557091,-0.22366,0.532792,-0.403409,0.864983,0.488498,0.428235,0.510335,0.174244,0.905888,0.919195,0.627141,0.899472,-0.20864,-0.547284,0.768046,0.996118,
-0.510919,0.890229,-0.0172257,0.570817,0.657364,0.91646,0.32654,0.695844,0.640139,0.638042,-0.295315,0.738189,-0.0972656,0.563762,0.59043,0.84136,0.734269,-0.177616,0.275921,0.588279,0.259151,-0.719796,0.537222,0.0927632,0.598542,-0.221728,0.55257,-0.363015,0.87716,0.449617,0.315895,0.477397,0.832774,0.903427,0.916151,0.682209,0.898523,-0.268976,-0.413124,0.833759,-0.576868,
-0.786766,0.958714,0.025647,0.596336,0.736766,0.938936,-0.07253,0.263908,0.889063,0.740394,-0.0623769,0.806343,-0.622767,0.701805,0.567244,0.878819,0.151786,-1.04685,0.771327,0.827707,0.418083,-1.14287,0.625756,0.209697,0.746008,0.00944257,0.651531,-0.577233,0.903317,0.60237,0.518812,0.612304,0.289865,0.918866,0.936548,0.730824,0.920744,0.892946,0.147867,0.839081,-0.437477,
-0.82654,0.919231,0.0033903,0.598214,0.736238,0.93693,-0.0223295,0.282538,0.714947,0.744477,-0.0523514,0.796901,-0.707729,0.667772,0.562887,0.881553,0.186256,-1.14169,0.385053,0.607802,0.342308,-1.14502,0.559551,0.220324,0.677816,0.0358729,0.645899,-0.545388,0.986583,0.564127,0.502297,0.604338,0.312778,0.994732,1.00354,0.748779,0.98961,-0.034889,-0.317445,0.877233,-0.431991,
-0.875684,0.943948,0.00362747,0.59486,0.737495,0.974335,-0.0275248,0.284159,0.714054,0.744532,-0.0494531,0.800364,-0.707492,0.669814,0.557109,1.00507,0.186949,-1.14583,0.374189,0.598688,0.353773,-1.15512,0.561903,0.228965,0.688225,0.0296755,0.646518,-0.559661,0.902643,0.585581,0.506234,0.602078,0.315567,0.922329,0.939337,0.744349,0.918064,-0.0225119,-0.341889,0.826595,-0.427762,
-0.815239,0.911246,-0.0270687,0.583759,1.00346,0.932756,-0.00998904,0.295607,0.74907,1.00399,-0.0226576,0.784141,-0.548997,0.655596,0.53405,0.870489,0.173624,-1.07109,0.337477,0.59001,0.39475,-0.986438,0.560354,0.200182,0.733576,0.881407,0.997194,-0.543693,0.912212,0.794149,0.49814,0.579967,0.275939,0.917262,0.93396,0.729931,0.918319,-0.0793836,-0.334761,0.818994,-0.451258,
-0.821436,0.914837,0.21152,0.724407,0.732993,0.942071,-0.0699417,0.27264,0.710937,0.739829,-0.0587678,0.796792,-0.716223,0.677014,0.540558,0.873861,0.163489,-1.13443,0.369486,0.587769,0.411939,-1.22178,0.535618,0.212286,0.681134,0.00123968,0.640011,-0.282392,0.901567,0.562632,0.814017,0.84229,0.280842,0.919668,0.936785,0.763307,0.916515,-0.0507291,-0.331407,0.813325,-0.435417,
-0.793474,0.914656,0.22346,0.675738,0.737094,0.943256,-0.0665513,0.265567,0.74856,0.741433,-0.0592964,0.956343,-0.705778,0.894167,0.751968,0.878673,0.170106,-1.0973,0.434743,0.615931,0.55,-1.07464,0.573296,0.214856,0.699945,0.0179547,0.650657,-0.529256,0.902296,0.591834,0.591706,0.705633,0.289865,0.921254,0.937733,0.908239,0.919067,-0.0156945,-0.20525,0.82089,-0.439264 };

char feature_words_arr[40][100] = {"agriculture", "bank", "banks", "bpd", "certificates", "company", "corp", "countries", "crude", "cts", "currency", "dlrs", "dollar", "exchange", "grain", "inc", "it", "japan", "japanese", "market", "mln", "more", "net", "official", "oil", "opec", "pct", "port", "prices", "rate", "rates", "shares", "shipping", "ships", "stg", "strike", "trade", "u", "union", "vs" };  
#endif

/* Checks if c is a low case alphabet */
bool is_low_alpha(char c) {
  if(c >= 97 && c <= 122) {
    return true;
  }
  return false;
}

bool is_high_alpha(char c) {
  if(c >= 65 && c <= 90) {
    return true;
  }
  return false;
}

/* Checks if the string is made off low case alphabets */
bool is_string_alpha(string s) {
  bool ret_val = true;
  for(int i = 0; i < s.length(); i++) {
    ret_val = ret_val && is_low_alpha(s[i]); 
  }
  return ret_val;
}

void convert_alpha_high_to_low(string &str) {
  for(int i = 0; i < str.size(); i++) {
    if(!is_low_alpha(str[i])) {
      if(is_high_alpha(str[i])){
	str[i] += 32;
      } else {
	assert(0);
      }
    }
  }

//  cout<<str<<endl;
}

vector<float> get_x(string line, vector<string> feature_words) {


  trie t;
  init_trie(&t);

  string str_to_add = "";
  for(int i = 0; i < line.size(); i++) {
    if(line[i] == ' ') {
      convert_alpha_high_to_low(str_to_add);
      assert(is_string_alpha(str_to_add));
      add_string(&t, str_to_add);
      str_to_add = "";
    } else {
      str_to_add += line[i];
    }
  }
  if(str_to_add != "") {
    convert_alpha_high_to_low(str_to_add);
    assert(is_string_alpha(str_to_add));
    add_string(&t, str_to_add);
  }

  vector<float> x;
  x.push_back(1);
  for(int i = 0; i < feature_words.size(); i++) {
    x.push_back(get_str_count(&t, feature_words[i]));
  }

  destroy_trie(&t);

  return x;
}

double hypothesis(vector<float> &x, vector<float> &theta) {
  double theta_t_x = 0;
  assert(x.size() == theta.size());
  for(int i = 0; i < x.size(); i++) {
    theta_t_x += theta[i] * x[i];
  }
  double sigmoid_ret_val =  ( 1 / ( 1 + exp(-theta_t_x) )); 

  //return (sigmoid_ret_val >= 0.5) ? 1 : 0;
  return sigmoid_ret_val;
}

int main() {

  int num_theta;
  num_theta = F + 1;

  vector<string> feature_words;

  for(int i = 0; i < F; i++) {
    feature_words.push_back(string(feature_words_arr[i]));
  }
  
  vector<vector<float> > theta;
  for(int i = 0; i < CLASS; i++) {
    vector<float> t;
    for(int j = 0; j < num_theta; j++) {
      t.push_back(theta_arr[i][j]);
    }
    theta.push_back(t);
  }
  
  int T;
  cin>>T;

  string s;
  getline(cin, s);
  
  while(T--) {
    string doc_str;
    getline(cin, doc_str);

    vector<float> x = get_x(doc_str, feature_words);

    vector<double> prob;
    
    for(int i = 0; i < CLASS; i++) {
      double p = hypothesis(x, theta[i]);
      prob.push_back(p);
    }
    
    float max = -1;
    int max_index = -1;
    /* Get highest probability */
    for(int i = 0; i < CLASS; i++) {
      if(prob[i] > max) {
	max = prob[i];
	max_index = i;
      }
    }

    assert(max_index != -1);

    cout<<max_index + 1<<endl;

  }

  return 0;
}
