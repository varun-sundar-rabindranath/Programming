#define WGS 256

/* Compute inplace inclusive prefix sum on the loaded local memory */
void calcPrefixSum( __local int *localMem,
					          __private int inputSize) {
	int threadid = get_local_id(0);
	int workGroupStartIndex = get_group_id(0) * WGS * 2;

    /* Bottom up part */
	int stride = 1;
	for(stride = 1; stride <= get_local_size(0); stride *= 2) {
				int index = (threadid + 1) * stride * 2 - 1;
				if(index + workGroupStartIndex < inputSize) {
					localMem[index] += localMem[index - stride];
				}
				barrier(CLK_LOCAL_MEM_FENCE);
	}	

	/*Top down part */
	for(stride = WGS/2; stride > 0; stride /= 2) {
			   int index = (threadid + 1) * stride * 2 - 1;
			   if(index + stride + workGroupStartIndex < inputSize) {
			   		localMem[index + stride] += localMem[index];
			   }
			   barrier(CLK_LOCAL_MEM_FENCE);
	}

}

__attribute((reqd_work_group_size(WGS, 1, 1)))
__kernel void prefixsum(
                        __global int *inputArray,
                        __private int inputSize,
                        __global int *globalCoop) {
  __local int localMem[WGS * 2];
  int globaloffset = get_group_id(0) * WGS * 2;
  int lidx = get_local_id(0);

  /* Load local memory */
  if(globaloffset + lidx < inputSize) {
    localMem[lidx] = inputArray[globaloffset + lidx];
  }
  if(globaloffset + lidx + WGS < inputSize) {
    localMem[lidx + WGS] = inputArray[globaloffset + lidx + WGS];
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  /* Calculate prefix sum */
  calcPrefixSum(localMem, inputSize);

  /* Store the prefix summed array to global */
  if(globaloffset + lidx < inputSize) {
    inputArray[globaloffset + lidx] = localMem[lidx]; 
  }
  if(globaloffset + lidx + WGS < inputSize) {
    inputArray[globaloffset + lidx + WGS] = localMem[lidx + WGS];
  }

  /*  Store the necessary values in globalcoop compute prefix sum
      for the entire array */
  if( globalCoop != 0) {
    unsigned short lastIndex = (get_group_id(0) == get_num_groups(0) - 1) ?
                               (inputSize % (WGS * 2)) - 1 : (WGS * 2) - 1; 
    globalCoop[get_group_id(0)] = localMem[lastElement];
  }
}

__attribute((reqd_work_group_size(WGS, 1, 1)))
__kernel void reductionKernel(
                              __global int *pickValsArray,
                              __global int *addToArray,
                              __private int addToArrayLength) {

  __local int localAddon;

  int lidx = get_local_id(0);  
  int globalOffset = (get_group_id(0) + 1) * WGS * 2;
  
  if(get_local_id(0) == 0) {
    localAddon = pickValsArray[get_group_id(0)];
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  int addOn = localAddon;
  
  if(globalOffset + lidx < addToArrayLength) {
    addToArray[globalOffset + lidx] += addOn;
  }

  if(globalOffset + lidx + WGS < addToArrayLength) {
    addToArray[globalOffset + lidx + WGS] += localAddon;
  }
}
