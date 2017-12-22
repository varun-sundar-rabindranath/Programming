#include <stdio.h>
#include <pthread.h>


#define C_A 1

typedef struct wow {
  int a;
  int b;
  int c;
  int d;
  pthread_mutex_t m; 
} wow;

static wow g = {.a = 2, .b = 3, .m = PTHREAD_MUTEX_INITIALIZER};

int main() {


    printf("wow - a %d , b %d , c %d , d %d \n", g.a, g.b, g.c, C_A);
    return 0;
}
