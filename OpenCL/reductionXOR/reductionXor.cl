#pragma OPENCL EXTENSION cl_amd_printf : enable

/* This WGS is not to be changed */
#define WGS 256

__attribute((reqd_work_group_size(16, 16, 1)))
__kernel void reductionXor(
                            __private unsigned char bit1,
                            __private unsigned char bit2,
                            __local unsigned short localMem[WGS * 2],
                            __private int inputSize /* Number of elements in localMem */
                            ) {

  int lidx = get_local_id(0) * 2;
  int stride = 1;

  /* Find inplace reductionXor */
  for( stride = 1; stride <= WGS; stride *= 2) {
              int index = lidx + stride 
  }


}

