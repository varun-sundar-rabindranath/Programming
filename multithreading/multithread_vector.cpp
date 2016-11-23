#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

pthread_mutex_t vector_lock;

void* vec_insert(void *arg) {
  vector<char>* in_vec = (vector<char>*)arg;

  unsigned long long int iter = 0;
  while(1) {
    /* When update */
    pthread_mutex_lock(&vector_lock);
    in_vec->push_back(iter % 256);
    pthread_mutex_unlock(&vector_lock);
    iter++;
  }
  return NULL;
}

void* read_vec(void *arg) {
  vector<char>* in_vec = (vector<char>*)arg;
  while(1) {
    /* Just keep copying !! will the compiler optimize it ?? */
    /* When vector copy */
    pthread_mutex_lock(&vector_lock);
    vector<char> copy_vec = (*in_vec);
    pthread_mutex_unlock(&vector_lock);

    /* FInd copy vec sum */
    unsigned long long int sum = 0;
    for(vector<char>::iterator it = copy_vec.begin(); it != copy_vec.end(); it++) {
      sum += *it;
      //*it = *it + 1;
    }
    cout<<"Size - "<<copy_vec.size()<<endl;
  }
  return NULL;
}


int main() {

  pthread_t t1, t2;

  vector<char> vec;

  int ret_val;

  pthread_mutex_init(&vector_lock, NULL);

  ret_val = pthread_create(&t1, NULL, &read_vec, (void *)&vec);
  assert(ret_val == 0);

  ret_val = pthread_create(&t2, NULL, &vec_insert, (void *)&vec);
  assert(ret_val == 0);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  pthread_mutex_destroy(&vector_lock);

  return 0;
}
