
#include "sceneintersect.h"

typedef float float4 __attribute__((ext_vector_type(4)));
typedef float float2 __attribute__((ext_vector_type(2)));

print_val_a() {
}

print_val_b() {
}

print_val_c() {
  print_val_a();
}

bool print_val_d() {
  print_val_a();
}

bool print_val_e() {
  print_val_a();
}

#define __kernel
#define __global
#define __global
__kernel void sample_kernel(__global int *a, __global int *b) {
  int c,d;
  if(print_val_d() == print_val_e()) {
    print_val_b();
  }

  float4 aass;
 bool hit = scene_intersect_AMD();
  
}
