#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <CL/opencl.h>

#define WGS 256

void printData(int numLevels,
               size_t prefixSum_globalWorkSizes[][3],
               size_t reduction_globalWorkSizes[][3],
               size_t localWorkSize[3]) {
  fprintf(stderr, " numLevels - %d \n", numLevels);

  fprintf(stderr, " Local worksizes - %lu %lu %lu \n", localWorkSize[0], localWorkSize[1], localWorkSize[2]);
  
  fprintf(stderr, " prefixSum - global work sizes \n");
  for(int i = 0; i <= numLevels; i++) {
    fprintf(stderr, "%lu - %lu - %lu \n", prefixSum_globalWorkSizes[i][0], prefixSum_globalWorkSizes[i][1], prefixSum_globalWorkSizes[i][2]);
  }

  fprintf(stderr, " reductionKernel - global work sizes \n");
  for(int i = numLevels-1; i >= 0; i--) {
    fprintf(stderr, "%lu - %lu - %lu \n", reduction_globalWorkSizes[i][0], reduction_globalWorkSizes[i][1], reduction_globalWorkSizes[i][2]);
  } 
}

char * getKernelSource(char *path) {

  FILE *fp = fopen(path, "r");
  fseek(fp, 0, SEEK_END);
  unsigned long long int fileSize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *kernelSource = (char *)malloc(fileSize + 1);
  assert(kernelSource != NULL);

 	assert( fileSize == fread(kernelSource, 1, fileSize, fp));
  kernelSource[fileSize] = '\0';
  return kernelSource;
}

/* Returns the execution time denoted by the event */ 
unsigned long long int getTime(cl_event event) {

  cl_int err = CL_SUCCESS;

  unsigned long long int startTime = 0, endTime = 0;

  err = clGetEventProfilingInfo(
                                event,
                                CL_PROFILING_COMMAND_START,
                                sizeof(unsigned long long int),
                                &startTime,
                                NULL);
  assert(err == CL_SUCCESS);

  err = clGetEventProfilingInfo(
                                event,
                                CL_PROFILING_COMMAND_END,
                                sizeof(unsigned long long int),
                                &endTime,
                                NULL);
  assert(err == CL_SUCCESS);

  return endTime - startTime;

}

bool doRandPrefixSum( int inputSize, 
                      cl_context *context, 
                      cl_program *program, 
                      cl_command_queue *queue) {
  
  cl_int err = CL_SUCCESS;

  int *hostInputArray = NULL;
  int *hostOutputArray = NULL;
  int *hostComputedOutput = NULL;

  /* Creating host input and output array */
  hostInputArray = (int *)malloc(inputSize * sizeof(int));
  assert(hostInputArray != NULL);
  hostOutputArray = (int *)malloc(inputSize * sizeof(int));
  assert(hostOutputArray != NULL);
  hostComputedOutput = (int *)malloc(inputSize * sizeof(int));
  assert(hostComputedOutput != NULL);

  // Filling host input array with random values
  for(int i = 0; i < inputSize; i++) {
    hostInputArray[i] = rand()%10;
  }

  /* Computing output in host */
  hostComputedOutput[0] = hostInputArray[0];
  for(int i = 1; i < inputSize; i++) {
    hostComputedOutput[i] = hostInputArray[i] + hostComputedOutput[i-1];
  }

  /* Creating device input buffer */
  cl_mem inputArray = clCreateBuffer( *context,
                                      CL_MEM_READ_WRITE,
                                      inputSize * sizeof(int),
                                      NULL,
                                      &err);
  assert(err == CL_SUCCESS);

  /* Write input data to GPU */
  err = clEnqueueWriteBuffer( *queue,
                              inputArray,
                              CL_TRUE,
                              0,
                              inputSize * sizeof(int),
                              hostInputArray,
                              0,
                              NULL,
                              NULL);
  assert(err == CL_SUCCESS);

  int numLevels = (int(log2(inputSize-1)))/9;

  cl_event psEvent[numLevels + 1];
  cl_event reductionEvent[numLevels];

  cl_kernel psKernel[numLevels + 1];
  cl_kernel reductionKernel[numLevels];

  cl_mem globalCoop[numLevels];
  cl_mem nullMem = NULL;

  // Create kernels
  for(int i = 0; i <= numLevels; i++) {
   psKernel[i] = clCreateKernel(*program, "prefixsum", &err);
   assert(err == CL_SUCCESS);

   if(i < numLevels) {
     reductionKernel[i] = clCreateKernel(*program, "reductionKernel", &err);
     assert(err == CL_SUCCESS);
   }
  } 

  /* Argument setting for prefix sum and reduction kernels */
  if(numLevels != 0) {
    int de = inputSize;
    for(int i = 0; i < numLevels; i++) {
      de = ((de - 1)/(WGS * 2)) + 1;
      globalCoop[i] = clCreateBuffer( *context,
                                      CL_MEM_READ_ONLY,
                                      de * sizeof(int),
                                      NULL,
                                      &err);
      assert(err == CL_SUCCESS);

    }

    for(int i = 0; i < numLevels; i++) {
      /* Setting globalCoop for prefix sum kernels */
      err = clSetKernelArg(psKernel[i], 2, sizeof(cl_mem), &globalCoop[i]);
      assert(err == CL_SUCCESS);

      /* Setting inputArray for prefix sum kernels */
      err = clSetKernelArg(psKernel[i + 1], 0, sizeof(cl_mem), &globalCoop[i]);
      assert(err == CL_SUCCESS);

      /* Setting pickValsArray for reduction kernel */
      err = clSetKernelArg(reductionKernel[i], 0, sizeof(cl_mem), &globalCoop[i]);
      assert(err == CL_SUCCESS);
    }

    /* Setting addToArray for reduction kernel */
    for(int i = 1; i < numLevels; i++) {
      err = clSetKernelArg(reductionKernel[i], 1, sizeof(cl_mem), &globalCoop[i-1]);
      assert(err == CL_SUCCESS);
    }

    /* Setting dataset size for reduction kernel */
    de = inputSize;
    for(int i = 0; i < numLevels; i++) {
      err = clSetKernelArg(reductionKernel[i], 2, sizeof(int), &de);
      assert(err == CL_SUCCESS); 
      de = ((de - 1) / (WGS * 2)) + 1;
    }

    /* Every arguments for has been set for the reduction kernels expect
     * the addToArray argument for reductionKernel[0]
     */
    err = clSetKernelArg(reductionKernel[0], 1, sizeof(cl_mem), &inputArray);
    assert(err == CL_SUCCESS);
  }

  /* Setting input size arguments for the prefix sum kernel */
  int de = inputSize;
  for(int i = 0; i <= numLevels; i++) {
    err = clSetKernelArg(psKernel[i], 1, sizeof(int), &de);
    assert(err == CL_SUCCESS);
    de = ((de - 1) / (WGS * 2)) + 1;
  }
  
  err = clSetKernelArg(psKernel[0], 0, sizeof(cl_mem), &inputArray);
  assert(err == CL_SUCCESS);

  err = clSetKernelArg(psKernel[numLevels], 2, sizeof(cl_mem), &nullMem);
  assert(err == CL_SUCCESS);

 
  /* Calculating global work sizes */
  size_t prefixSum_globalWorkSizes[numLevels + 1][3];
  size_t reduction_globalWorkSizes[numLevels][3];

  /* Global work sizes for prefix sum kernel */
  de = inputSize;
  for(int i = 0; i <= numLevels; i++) {
    prefixSum_globalWorkSizes[i][0] = (((de - 1) / (WGS * 2)) + 1) * (WGS) ;
    prefixSum_globalWorkSizes[i][1] = 1;
    prefixSum_globalWorkSizes[i][2] = 1;

    de = ((de - 1) / (WGS * 2)) + 1;
  }

  /* Global work sizes for reduction kernel */
  for(int i = 0; i < numLevels; i++) {
    reduction_globalWorkSizes[i][0] = prefixSum_globalWorkSizes[i][0] - 256;
    reduction_globalWorkSizes[i][1] = 1;
    reduction_globalWorkSizes[i][2] = 1;
  }
  
  size_t localWorkSize[3] = {WGS, 1, 1};
  
  /* Enqueue kernels */
  /* First enqueue prefix sum kernels */
  for( int i = 0; i <= numLevels; i++) {
    err = clEnqueueNDRangeKernel(*queue, psKernel[i], 3, NULL, prefixSum_globalWorkSizes[i], localWorkSize, 0, NULL, &psEvent[i]);
    assert(err == CL_SUCCESS);
  }

  /* Second enqueue reduction kernels in the reverse order */
  for(int i = numLevels - 1; i >= 0; i--) {
    err = clEnqueueNDRangeKernel(*queue, reductionKernel[i], 3, NULL, reduction_globalWorkSizes[i], localWorkSize, 0, NULL, &reductionEvent[i]);
    assert(err == CL_SUCCESS);
  }

   /* Read Output data from GPU */
  err = clEnqueueReadBuffer(  *queue,
                              inputArray,
                              CL_TRUE,
                              0,
                              inputSize * sizeof(int),
                              hostOutputArray,
                              0,
                              NULL,
                              NULL);
  assert(err == CL_SUCCESS);

  bool retval = 1;
  
  // Check output correctness of GPU computation
  for(int i = 0; i < inputSize; i++) {
    if(hostComputedOutput[i] != hostOutputArray[i]) {
      retval = 0;
      break;
    }
  }
 
  if(retval) {
    /* Print kernel times */
    unsigned long long int psTimes[numLevels + 1];
    unsigned long long int reductionTimes[numLevels];
    unsigned long long int totalPSTime = 0, totalReductionTime = 0;
    for(int i = 0; i <= numLevels; i++) {
      psTimes[i] = getTime(psEvent[i]);
      totalPSTime += psTimes[i];
      if(i < numLevels) {
        reductionTimes[i] = getTime(reductionEvent[i]);
        totalReductionTime += reductionTimes[i];
      }
    }

    fprintf(stderr, "%d - %llu - %llu - %llu \n", inputSize, totalReductionTime + totalPSTime, totalPSTime, totalReductionTime);

  } else {
    fprintf(stderr, "%d - Incorrect output \n", inputSize);
#if 0
    fprintf(stderr, "Input - hostComputed - GPU computed \n");
    for(int i = 0; i < inputSize; i++) {
      fprintf(stderr, "%d - %d - %d \n", hostInputArray[i], hostComputedOutput[i], hostOutputArray[i]);
    }
#endif
  }

  /* cl_mem release */
  clReleaseMemObject(inputArray);
  for(int i = 0; i < numLevels; i++) {
    clReleaseMemObject(globalCoop[i]);
  }

  /* Release kernels */
  for(int i = 0; i <= numLevels; i++) {
    clReleaseKernel(psKernel[i]);
    if(i < numLevels) {
      clReleaseKernel(reductionKernel[i]);
    }
  }

  /* Release host memory */
  free(hostComputedOutput);
  free(hostInputArray);
  free(hostOutputArray);

  return retval;
}

int main(int argc, char *argv[]) {

  if(argc != 2) {
    fprintf(stderr, " Insufficient arguments \n");
    return 0;
  }

  int tillVal = atoi(argv[1]);

  srand(time(NULL));

  cl_platform_id cpPlatform;        // OpenCL platform
  cl_device_id device_id;           // device ID
  cl_context context;               // context
  cl_command_queue queue;           // command queue
  cl_program program;               // program
  
  cl_int err = CL_SUCCESS;
  
  // Bind to platform
  err |= clGetPlatformIDs(1, &cpPlatform, NULL);
  
  // Get ID for the device
  err |= clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
  
  assert(err == CL_SUCCESS);
  
  // Create a context 
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  assert(err == CL_SUCCESS);
  
  // Create a command queue
  queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);
  assert(err == CL_SUCCESS);
  
  /* Get program from file */
  char path[100] = "/home/mcw/varun/programming/prefixSumKernel.cl";
  char *kernelSource = getKernelSource(path);

  // Create the compute program from the source buffer
  program = clCreateProgramWithSource(context, 1,
                          (const char **) & kernelSource, NULL, &err);
	assert(err == CL_SUCCESS);
 
  // Build the program executable
  err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	// Output build log incase of error 
	if(err != CL_SUCCESS) {
		char buildLog[10000];
		err = clGetProgramBuildInfo(program,
									device_id,
									CL_PROGRAM_BUILD_LOG,
									10000,
									buildLog,
									NULL);
		fprintf(stderr, "clBuildProgram error - %d \n", err);
		fprintf(stderr, "BuildLog - \n %s ", buildLog);
		exit(0);
	}
 
  /* All setup done */

  fprintf(stderr, "InputSize - TotalTime - PrefixSumTime - ReductionTime \n");

  for(int i = 262656; i <= tillVal; i++) {
    doRandPrefixSum(i, &context, &program, &queue);
  }

  free(kernelSource);
  return 0;
}

