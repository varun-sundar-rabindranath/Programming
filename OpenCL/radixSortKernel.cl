
#pragma OPENCL EXTENSION cl_amd_printf : enable

#define WGS 256

/* calcPrefixSum - integer version;
 * Compute inplace inclusive prefix sum on the loaded local memory
 * Each thread takes care of 2 values 
 */
void calcPrefixSum( __local int *localMem,
		                __private int inputSize,
                    __private int inputArrayStartIndex) {
	int threadid = get_local_id(0);
	int workGroupStartIndex = inputArrayStartIndex + get_group_id(0) * WGS * 2;

    /* Bottom up part */
	int stride = 1;
	for(stride = 1; stride <= get_local_size(0); stride *= 2) {
				int index = (threadid + 1) * stride * 2 - 1;
				if(index + workGroupStartIndex < inputArrayStartIndex + inputSize) {
					localMem[index] += localMem[index - stride];
				}
				barrier(CLK_LOCAL_MEM_FENCE);
	}	

	/*Top down part */
	for(stride = WGS/2; stride > 0; stride /= 2) {
			   int index = (threadid + 1) * stride * 2 - 1;
			   if(index + stride + workGroupStartIndex < inputArrayStartIndex + inputSize) {
			   		localMem[index + stride] += localMem[index];
			   }
			   barrier(CLK_LOCAL_MEM_FENCE);
	}

}

/* calcPrefixSum - unsigned short version
 * Compute inplace inclusive prefix sum on the loaded local memory
 * Each thread takes care of 2 values 
 */
void calcPrefixSum_US( __local unsigned short *localMem,
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


/* Each thread takes care of 2 values */
__attribute((reqd_work_group_size(WGS, 1, 1)))
__kernel void prefixsum(
                        __global int *inputArray,
                        __private int inputSize,
                        __global int *globalCoop,
                        __private int inputArrayStartIndex,
                        __private int numLevels) {

  __local int localMem[WGS * 2];
  int globalOffset = inputArrayStartIndex + get_group_id(0) * WGS * 2;
  int lidx = get_local_id(0);

  if(lidx == 0) {
    printf("localmem %p \n", localMem);
    int iter = 0;
    for(iter = 0; iter < WGS ; iter++) {
      printf(" %d ", localMem[iter]);
    }
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  /* Load local memory */
  if(globalOffset + lidx < inputArrayStartIndex + inputSize) {
    localMem[lidx] = inputArray[globalOffset + lidx];
  }
  if(globalOffset + lidx + WGS < inputArrayStartIndex + inputSize) {
    localMem[lidx + WGS] = inputArray[globalOffset + lidx + WGS];
  }
  barrier(CLK_LOCAL_MEM_FENCE);


  /* Calculate prefix sum */
  calcPrefixSum(localMem, inputSize, inputArrayStartIndex);

  /* Store the prefix summed array to global */
  if(globalOffset + lidx < inputArrayStartIndex + inputSize) {
    inputArray[globalOffset + lidx] = localMem[lidx];
  }
  if(globalOffset + lidx + WGS < inputArrayStartIndex + inputSize) {
    inputArray[globalOffset + lidx + WGS] = localMem[lidx + WGS];
  }

  if(numLevels == 0) {
    if(lidx == 0) {
      int lastElement = (inputSize - 1) % (WGS * 2);
      inputArray[inputArrayStartIndex + inputSize] += localMem[lastElement];
    }
    return;
  }
  /*  Store the necessary values in globalcoop compute prefix sum
      for the entire array */
  if( globalCoop != 0) {
    int numWorkGroupsReqd = ((inputSize - 1) / (WGS * 2)) + 1;
    int lastElement = (get_group_id(0) == numWorkGroupsReqd - 1) ?
                      ((inputSize - 1) % (WGS * 2))  : (WGS * 2) - 1;
    globalCoop[get_group_id(0)] = localMem[lastElement];
  }
}

/* Each thread takes care of 2 values */
/* Reduction kernel modified for radix sort */
__attribute((reqd_work_group_size(WGS, 1, 1)))
__kernel void reductionKernel(
                              __global int *pickValsArray,
                              __global int *addToArray,
                              __private int addToArrayLength,
                              __private char lastReductionkernel,
                              __private int addToArrayStartIndex /* Valid only for last reduction kenrel*/
                              ) {

  __local int localAddon;

  int lidx = get_local_id(0);
  int globalOffset = addToArrayStartIndex + (get_group_id(0) + 1) * WGS * 2;

  if(get_local_id(0) == 0) {
    localAddon = pickValsArray[get_group_id(0)];
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  int addOn = localAddon;

  if(globalOffset + lidx < addToArrayStartIndex + addToArrayLength) {
    addToArray[globalOffset + lidx] += addOn;
  }

  if(globalOffset + lidx + WGS < addToArrayStartIndex + addToArrayLength) {
    addToArray[globalOffset + lidx + WGS] += addOn;
  }

  /* This part has nothing to do with core prefix sum logic,
   * But when prefix sum is being used for radix sorting this
   * this part is essential
   */
  if(lastReductionkernel) {
    barrier(CLK_GLOBAL_MEM_FENCE);
    if(lidx == 0 && get_group_id(0) == get_num_groups(0) - 1) {
      addToArray[addToArrayStartIndex + addToArrayLength] += addToArray[ addToArrayStartIndex + addToArrayLength - 1];
    }
  }
}

/* Radix sort kernel */
/* Each thread takes care of 2 values */
__attribute__((reqd_work_group_size(WGS, 1, 1)))
__kernel void radixSort(
                        __global int *inputArray, /* Individual chunks are sorted into inputArray */
                        __global int *outputArray, /* If only this kernel is enough for sorting - we store the result in this array */
                        __private int inputSize,
                        __private int numTypes,
                        __private int numBits,
                        __global int *globalCoop,
                        __local unsigned short *startIndex, /* To count types present in localMemory */ 
                        __local unsigned short *endBeforeIndex /* To count types present in localMemory */
                        ) {

  if(numTypes == 1) {
    return;
  }

  unsigned short lidx = get_local_id(0);
  int globalOffset = get_group_id(0) * WGS * 2;
  unsigned short lastIndex = (get_group_id(0) == get_num_groups(0) - 1) ?
                               ((inputSize - 1) % (WGS * 2)) : (WGS * 2) - 1;

  /* Used by core radixSort algorithm */
  __local int localMem[WGS * 2];
  __local unsigned short rearrangeIndex[WGS * 2];


  /* Initialize local memories */
  int iterator = 0;
  for( iterator = 0; iterator < numTypes; iterator = iterator + WGS) {
                if(iterator + lidx < numTypes) {
                  startIndex[iterator + lidx] = 0;
                  endBeforeIndex[iterator + lidx] = 0;
                }
  }

  /* Load local memory */
  if(globalOffset + lidx < inputSize) {
    localMem[lidx] = inputArray[globalOffset + lidx];
  }
  if(globalOffset + lidx + WGS < inputSize) {
    localMem[lidx + WGS] = inputArray[globalOffset + lidx + WGS];
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  char wantedBit = 1;

  /* Sort localMem */
  while(numBits > 0) {

    char bit1 = 0, bit2 = 0;
    int data1 = 0, data2 = 0;
    /* Load current bit of interest into local memory
     * Since we wish to find count of '0' bit. We set all '0' bits to 1.
     */
    if(globalOffset + lidx < inputSize) {
      data1 = localMem[lidx];
      bit1 = data1 & wantedBit;
      rearrangeIndex[lidx] = !bit1;
    }
    if(globalOffset + lidx + WGS < inputSize) {
      data2 = localMem[lidx + WGS];
      bit2 = data2 & wantedBit;
      rearrangeIndex[lidx + WGS] = !bit2;
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    /* Do prefix sum to find number of 0's till any index */
    calcPrefixSum_US(rearrangeIndex, inputSize);

    unsigned short total = 0;
    unsigned short index;

    total = rearrangeIndex[lastIndex];

    /* rearrange data in localMem */
    if(globalOffset + lidx < inputSize) {
      index = bit1? total + lidx - rearrangeIndex[lidx] : rearrangeIndex[lidx] - 1;
      localMem[index] = data1;
    }
    if(globalOffset + lidx + 256 < inputSize) {
      index = bit2? total + (lidx + 256) - rearrangeIndex[lidx + 256] : rearrangeIndex[lidx + 256] - 1;
      localMem[index] = data2;
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    wantedBit = wantedBit << 1;
    numBits = numBits - 1;
  }

  if(inputSize <= WGS * 2) {
    /* Store results into output array */
    if(globalOffset + lidx < inputSize) {
      outputArray[globalOffset + lidx] = localMem[lidx];
    }
    if(globalOffset + lidx + WGS < inputSize) {
      outputArray[globalOffset + lidx + WGS] = localMem[lidx + WGS];
    }
    return;
  }

  /* Store sorted array to global */
  if(globalOffset + lidx < inputSize) {
    inputArray[globalOffset + lidx] = localMem[lidx];
  }
  if(globalOffset + lidx + WGS < inputSize) {
    inputArray[globalOffset + lidx + WGS] = localMem[lidx + WGS];
  }
  barrier(CLK_GLOBAL_MEM_FENCE);

  /* get count of each type to store in global memory */
  if(lidx < lastIndex) {
    if(localMem[lidx] != localMem[lidx+1]) {
      startIndex[localMem[lidx + 1]] = lidx + 1;
      endBeforeIndex[localMem[lidx]] = lidx + 1;
    }
  }

  if(lidx + 256 < lastIndex) {
    if(localMem[lidx + 256] != localMem[lidx + 256 + 1]) {
      startIndex[localMem[lidx + 256 + 1]] = lidx + 256 + 1;
      endBeforeIndex[localMem[lidx + 256]] = lidx + 256 + 1;
    }
  }

  if(lidx == 0) {
    endBeforeIndex[localMem[lastIndex]] = lastIndex + 1;
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  /* Store count of types into global cooperation memory */
  for( iterator = 0; iterator < numTypes; iterator = iterator + WGS) {
                if(iterator + lidx < numTypes) {
                  globalCoop[(get_num_groups(0) * (iterator + lidx)) + get_group_id(0)] = 
                               endBeforeIndex[iterator + lidx] - startIndex[iterator + lidx];

                }
  }

  if(get_group_id(0) == 0 && lidx == 0) {
    printf("last element index - %d \n", (get_num_groups(0) * numTypes) + 1);
    globalCoop[(get_num_groups(0) * numTypes)] = 0;
  }

}

/* Final rearrange kernel
 * Each thread takes care of 2 values
 */
__attribute__((reqd_work_group_size(WGS, 1, 1)))
__kernel void rearrangeKernel(
                              __global int *inputArray,
                              __global int *outputArray,
                              __global int *globalCoop,
                              __local int *typeOffsets, /* Array of size numTypes */
                              __local unsigned short *startIndex, /* Array of size numTypes */
                              __private int numTypes,
                              __private int inputSize
                              ) {

  unsigned char lidx = get_local_id(0);
  int globalOffset = get_group_id(0) * WGS * 2;
  int lastIndex = (get_group_id(0) == get_num_groups(0) - 1) ?
                      ((inputSize - 1) % (WGS * 2))  : (WGS * 2) - 1;

  __local int localMem[WGS * 2];

  /* Load input to local memory */
  if(globalOffset + lidx < inputSize) {
    localMem[lidx] = inputArray[globalOffset + lidx];
  }
  if(globalOffset + lidx + WGS < inputSize) {
    localMem[lidx + WGS] = inputArray[globalOffset + lidx + WGS];
  }

  int iterator = 0;
  if(get_group_id(0) != 0) {
    for( iterator = 0; iterator < numTypes; iterator = iterator + WGS) {
      /* Initialize local memories */
      if(iterator + lidx < numTypes) {
        startIndex[iterator + lidx] = 0;
      }

      /* Load typeOffsets */
      if(iterator + lidx < numTypes) {
        typeOffsets[iterator + lidx] = globalCoop[(get_num_groups(0) * (iterator + lidx))
                                                  + get_group_id(0) - 1];
      }
    }
  } else {
    for(iterator = 1; iterator < numTypes; iterator = iterator + WGS) {
      /* Initialize local memories */
      if(iterator + lidx < numTypes) {
        startIndex[iterator + lidx] = 0;
      }

      /* Load typeOffsets */
      if(iterator + lidx < numTypes) {
        typeOffsets[iterator + lidx] = globalCoop[(get_num_groups(0) * (iterator + lidx))
                                                  + get_group_id(0) - 1];
      }
    }
    if(lidx == 0) {
      startIndex[0] = 0;
      typeOffsets[0] = 0;
    }
  }

  barrier(CLK_LOCAL_MEM_FENCE);

  if(lidx < lastIndex) {
    if(localMem[lidx] != localMem[lidx+1]) {
      startIndex[localMem[lidx + 1]] = lidx + 1;
    }
  }

  if(lidx + 256 < lastIndex) {
    if(localMem[lidx + 256] != localMem[lidx + 256 + 1]) {
      startIndex[localMem[lidx + 256 + 1]] = lidx + 256 + 1;
    }
  }

  if(lidx == 0) {
    startIndex[localMem[0]] = 0;
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  int globalIndex1 = 0;
  int globalIndex2 = 0;

  if(globalOffset + lidx < inputSize) {
    globalIndex1 = (lidx - startIndex[localMem[lidx]]) + typeOffsets[localMem[lidx]];
    outputArray[globalIndex1] = localMem[lidx];
  }
  if(globalOffset + lidx + 256 < inputSize) {
    globalIndex2 = (lidx + 256 - startIndex[localMem[lidx + 256]]) + typeOffsets[localMem[lidx + 256]];
    outputArray[globalIndex2] = localMem[lidx + 256];
  }
}
