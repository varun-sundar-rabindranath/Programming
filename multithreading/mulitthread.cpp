#include <iostream>
#include <cassert>
#include <pthread.h>
#include <unistd.h>

using namespace std;

#define NUM_THREADS 100


void dummy_callback(int arg1, void* arg2) {
  cout<<"Dummy callback ME passed - "<<arg1<<endl;
  cout<<"Dummy callback User passed - "<<*((int *)arg2)<<endl;
  while(1) {}
}

void *run_callback(void *arg) {
  int pass_frm_here = 4;
  void (*cb)(int, void*) = (void (*)(int, void*))arg;
  cb(2, &pass_frm_here);
  return NULL;
}

int main() {

  pthread_t t[NUM_THREADS];
  pthread_attr_t thread_attr;
  
  pthread_attr_init(&thread_attr);
  pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

  for(int iter = 0; iter < NUM_THREADS; iter++) {
    int ret_val = pthread_create(&t[iter], &thread_attr, &run_callback, (void *)&dummy_callback);
    assert(ret_val == 0 && "Cannot create thread");
  }

  pthread_attr_destroy(&thread_attr);


#if 0
  for(int iter = 0; iter < NUM_THREADS; iter++) {
    pthread_join(t[iter], NULL);
  }
#endif

  return 0;
}
