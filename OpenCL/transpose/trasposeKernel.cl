__attribute((reqd_work_group_size(16, 16, 1)))
__kernel void transposeKernel(
                              __global int *inputMatrix,
                              __global int *outputMatrix,
                              __private int rows,
                              __private int cols
                              ) {
  __local int localMem[256];

  unsigned char lastLocalIndexX = (get_num_groups(0) - 1 == get_group_id(0)) ? 
                                  (cols - 1) % 16 : 16;
  unsigned char lastLocalIndexY = (get_num_groups(1) - 1 == get_group_id(1)) ? 
                                  (rows - 1) % 16 : 16;

  int gidx = get_global_id(0);
  int gidy = get_global_id(1);
  unsigned char lidx = get_local_id(0);
  unsigned char lidy = get_local_id(1);
  int outputOffset = get_group_id(0) * (rows * 16) + (get_group_id(1) * 16);

  if(lidx <= lastLocalIndexX && lidy <= lastLocalIndexY) {
    /* Load input into local memory */
    /* Perform transpose */
    localMem[lidx * 16 + lidy] = inputMatrix[gidy * cols + gidx];
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  if(lidx <= lastLocalIndexY && lidy <= lastLocalIndexX) {
    /* Place back the result */
    outputMatrix[outputOffset + (lidy * rows + lidx)] = localMem[lidy * 16 + lidx];
  }
}
