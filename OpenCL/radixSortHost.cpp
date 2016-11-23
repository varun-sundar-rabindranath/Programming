#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <CL/opencl.h>

/* For CPU sort */
#include <algorithm>
#include <vector>

#define WGS 256
#define INPUTSIZE 16768
#define NUMTYPES 8

#define DEBUG 1

using namespace std;

size_t localWorkSize[3] = {WGS, 1, 1};

bool compFunction(int i, int j) {
  return i<j;
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

size_t getMemObjectSize(cl_mem *buffer) {

  cl_int err = CL_SUCCESS;
  size_t retval = 0;

  err = clGetMemObjectInfo( *buffer,
                            CL_MEM_SIZE,
                            sizeof(size_t),
                            &retval,
                            NULL);
  assert(err == CL_SUCCESS);

  return retval;
}

bool setUpPrefixSumMemAndInit(
                              cl_mem *inputBuffer,
                              int inputSize,
                              int numLevels, /* num Levels based on radixSortWGS */
                              int startIndexLevel0,
                              cl_mem *globalCoopPrefixSum,
                              cl_kernel *psKernel,
                              cl_kernel *reductionKernel,
                              cl_program *program,
                              cl_context *context
                              )
{
  bool retval = 1;
  cl_int err = 0;
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
      globalCoopPrefixSum[i] = clCreateBuffer(*context,
                                              CL_MEM_READ_ONLY,
                                              de * sizeof(int),
                                              NULL,
                                              &err);
      assert(err == CL_SUCCESS);

    }

    for(int i = 0; i < numLevels; i++) {
      /* Setting globalCoop for prefix sum kernels */
      err = clSetKernelArg(psKernel[i], 2, sizeof(cl_mem), globalCoopPrefixSum[i]);
      assert(err == CL_SUCCESS);

      /* Setting inputArray for prefix sum kernels */
      err = clSetKernelArg(psKernel[i + 1], 0, sizeof(cl_mem), globalCoopPrefixSum[i]);
      assert(err == CL_SUCCESS);

      /* Setting numLevels argument */
      err = clSetKernelArg(psKernel[i + 1], 4, sizeof(int), &numLevels);
      assert(err == CL_SUCCESS);

      /* Setting up inputArrayStartIndex */
      int startIndex = 0;
      err = clSetKernelArg(psKernel[i + 1], 3, sizeof(int), &startIndex);
      assert(err == CL_SUCCESS);

      /* Setting pickValsArray for reduction kernel */
      err = clSetKernelArg(reductionKernel[i], 0, sizeof(cl_mem), globalCoopPrefixSum[i]);
      assert(err == CL_SUCCESS);
    }

    /* Setting addToArray for reduction kernel */
    for(int i = 1; i < numLevels; i++) {
      err = clSetKernelArg(reductionKernel[i], 1, sizeof(cl_mem), globalCoopPrefixSum[i-1]);
      assert(err == CL_SUCCESS);
    }

    /* Setting dataset size for reduction kernel */
    de = inputSize;
    for(int i = 0; i < numLevels; i++) {
      err = clSetKernelArg(reductionKernel[i], 2, sizeof(int), &de);
      assert(err == CL_SUCCESS); 
      de = ((de - 1) / (WGS * 2)) + 1;
    }

    for(int i = 1; i < numLevels - 1; i++) {
      /* Setting false for last reduction kernel argument */
      char falseVal = 0;
      err = clSetKernelArg(reductionKernel[i], 3, sizeof(char), &falseVal); 
      assert(err == CL_SUCCESS);

      /* Setting addToArrayStartIndex */
      int startIndex = 0;
      err = clSetKernelArg(reductionKernel[i], 4, sizeof(int), &startIndex);
    }

    char trueVal = 1;
    err = clSetKernelArg(reductionKernel[0], 3, sizeof(char), &trueVal);
    assert(err == CL_SUCCESS);

    err = clSetKernelArg(reductionKernel[0], 4, sizeof(int), &startIndexLevel0);
    assert(err == CL_SUCCESS);

    /* Every arguments for has been set for the reduction kernels expect
     * the addToArray argument for reductionKernel[0]
     */
    err = clSetKernelArg(reductionKernel[0], 1, sizeof(cl_mem), inputBuffer);
    assert(err == CL_SUCCESS);
  }

  /* Setting input size arguments for the prefix sum kernel */
  int de = inputSize;
  for(int i = 0; i <= numLevels; i++) {
    err = clSetKernelArg(psKernel[i], 1, sizeof(int), &de);
    assert(err == CL_SUCCESS);
    de = ((de - 1) / (WGS * 2)) + 1;
  }
  
  err = clSetKernelArg(psKernel[0], 0, sizeof(cl_mem), inputBuffer);
  assert(err == CL_SUCCESS);

  err = clSetKernelArg(psKernel[numLevels], 2, sizeof(cl_mem), &nullMem);
  assert(err == CL_SUCCESS);

  err = clSetKernelArg(psKernel[0], 3, sizeof(int), &startIndexLevel0);
  assert(err == CL_SUCCESS);

  err = clSetKernelArg(psKernel[0], 4, sizeof(int), &numLevels);
  assert(err == CL_SUCCESS);
  
  return retval;
}

bool setUpRadixSortMemAndInit(cl_mem *inputArray,
                              cl_mem *outputArray,
                              cl_mem *globalCoop,
                              cl_context *context,
                              int inputSize,
                              int numTypes) {

  cl_int err = 0;
  size_t globalCoopSize = (((inputSize - 1) / (WGS * 2)) + 1) * numTypes + 1;

  /* Creating device input buffer */
  *inputArray = clCreateBuffer( *context,
                                CL_MEM_WRITE_ONLY,
                                inputSize * sizeof(int),
                                NULL,
                                &err);
  assert(err == CL_SUCCESS);

  /* Creating device output buffer */
  *outputArray = clCreateBuffer( *context,
                                 CL_MEM_READ_ONLY,
                                 inputSize * sizeof(int),
                                 NULL,
                                 &err);
  assert(err == CL_SUCCESS);

  /* Creating global cooperation buffer */
  *globalCoop = clCreateBuffer( *context,
                                CL_MEM_READ_WRITE,
                                globalCoopSize * sizeof(int),
                                NULL,
                                &err);
  assert(err == CL_SUCCESS);

  return 1;
}

bool doRandRadixSort( int inputSize,
                      cl_context *context,
                      cl_program *program,
                      cl_command_queue *queue) {

  int numTypes = NUMTYPES;
  assert(numTypes > 0);

  /* Already sorted */
  if(numTypes == 1) { return true; }

  bool retval = 1;
  cl_int err = CL_SUCCESS;

  /* Declaring host memory */
  int *hostInputArray = NULL;
  int *gpuOutput = NULL;

  /* Creating host input */
  hostInputArray = (int *)malloc(inputSize * sizeof(int));
  assert(hostInputArray != NULL);

  /* Creating output array to store GPU computed results */
  gpuOutput = (int *)malloc(inputSize * sizeof(int));
  assert(gpuOutput != NULL);

  // Filling host input array with random values
  for(int i = 0; i < inputSize; i++) {
    hostInputArray[i] = rand()%NUMTYPES;
  }

  // Creating output vector to contain the CPU computed results 
  vector<int> cpuOutput(hostInputArray, hostInputArray + inputSize); 

  /* CPU sort */
  sort(cpuOutput.begin(), cpuOutput.end());

  int numBits = ((int)log2(numTypes - 1)) + 1;
  int radixSortWGS = ((inputSize - 1) / (WGS * 2)) + 1;
  int numLevels = 0;
  if(radixSortWGS == 1) {
    numLevels = 0;
  } else {
    numLevels = (int(log2((radixSortWGS)-1)))/9; // For prefix sum - We need log base 512
  }
  size_t globalCoopSizeRadixSort = radixSortWGS * NUMTYPES + 1;

  /* Declaring events and kernels */
  cl_kernel psKernel[numTypes][numLevels + 1];
  cl_kernel reductionKernel[numTypes][numLevels];
  cl_kernel sortKernel;
  cl_kernel rearrangeKernel;

  cl_event psEvent[numTypes][numLevels + 1];
  cl_event reductionEvent[numTypes][numLevels];
  cl_event sortKernelEvent;
  cl_event rearrangeKernelEvent;

  /* Declaring all cl_mem(s) */
  cl_mem inputArray, outputArray, globalCoopRadixSort;
  cl_mem globalCoopPrefixSum[numTypes][numLevels];

  /* Sets up all the cl_mem used by the radixSort kernel 
   * and initialises necessary memories 
   */
  assert(setUpRadixSortMemAndInit( &inputArray,
                  &outputArray,
                  &globalCoopRadixSort,
                  context,
                  inputSize,
                  numTypes));

  /* Create radix sort kernel */
  sortKernel = clCreateKernel( *program, "radixSort", &err); 
  assert(err == CL_SUCCESS);

  /* set radix sort kernel arguments */
  err |= clSetKernelArg(sortKernel, 0, sizeof(cl_mem), &inputArray);
  err |= clSetKernelArg(sortKernel, 1, sizeof(cl_mem), &outputArray);
  err |= clSetKernelArg(sortKernel, 2, sizeof(int), &inputSize);
  err |= clSetKernelArg(sortKernel, 3, sizeof(int), &numTypes); 
  err |= clSetKernelArg(sortKernel, 4, sizeof(int), &numBits);
  err |= clSetKernelArg(sortKernel, 5, sizeof(cl_mem), &globalCoopRadixSort);
  err |= clSetKernelArg(sortKernel, 6, numTypes * sizeof(unsigned short), NULL); // Local startIndex
  err |= clSetKernelArg(sortKernel, 7, numTypes * sizeof(unsigned short), NULL); // Local endIndex 
  assert(err == CL_SUCCESS);
  
  for( int i = 0; i < numTypes; i++) {
  /* Sets up all the cl_mem used for finding prefix sum -
   * Also creates kernels required for prefix sum and sets
   * their arguments
   */
    assert(setUpPrefixSumMemAndInit(&globalCoopRadixSort,
                                    radixSortWGS,
                                    numLevels,
                                    i * radixSortWGS,
                                    &globalCoopPrefixSum[i][0],
                                    &psKernel[i][0],
                                    &reductionKernel[i][0],
                                    program,
                                    context));
  }

  /* Create rearrangeKernel */
  rearrangeKernel = clCreateKernel(*program, "rearrangeKernel", &err);
  assert(err == CL_SUCCESS);

  /* Set arguments for rearrange kernel */
  err |= clSetKernelArg(rearrangeKernel, 0, sizeof(cl_mem), &inputArray);
  err |= clSetKernelArg(rearrangeKernel, 1, sizeof(cl_mem), &outputArray);
  err |= clSetKernelArg(rearrangeKernel, 2, sizeof(cl_mem), &globalCoopRadixSort);
  err |= clSetKernelArg(rearrangeKernel, 3, numTypes * sizeof(int), NULL); // typeOffsets
  err |= clSetKernelArg(rearrangeKernel, 4, numTypes * sizeof(unsigned short), NULL); // startIndex
  err |= clSetKernelArg(rearrangeKernel, 5, sizeof(int), &numTypes);
  err |= clSetKernelArg(rearrangeKernel, 6, sizeof(int), &inputSize);
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

  /* Calculating global work sizes */
  size_t radixSortGlobalWorkGroupSize[3] = {(size_t)radixSortWGS * WGS, 1, 1}; 
  size_t prefixSumGlobalWorkGroupSize[numLevels + 1][3];
  size_t reductionKernelGlobalWorkGroupSize[numLevels][3]; 
  size_t rearrangeKernelGlobalWorkGroupSize[3] = {(size_t)radixSortWGS * WGS, 1, 1};
  
  /* Global work sizes for prefix sum kernel */
  int de = radixSortWGS;
  for(int i = 0; i <= numLevels; i++) {
    de = ((de - 1) / (WGS * 2)) + 1;

    prefixSumGlobalWorkGroupSize[i][0] = de * (WGS);
    prefixSumGlobalWorkGroupSize[i][1] = 1;
    prefixSumGlobalWorkGroupSize[i][2] = 1;
  }

  /* Global work sizes for reduction kernel */
  for(int i = 0; i < numLevels; i++) {
    reductionKernelGlobalWorkGroupSize[i][0] = prefixSumGlobalWorkGroupSize[i][0] - 256;
    reductionKernelGlobalWorkGroupSize[i][1] = 1;
    reductionKernelGlobalWorkGroupSize[i][2] = 1;
  }
  
#if DEBUG
  /* Print data before kernel execution */
  /* Print values */
  fprintf(stderr, "numTypes - %d \n", numTypes);
  fprintf(stderr, "numLevels - %d \n", numLevels);
  fprintf(stderr, "numBits - %d \n", numBits);
  fprintf(stderr, "radixSortWGS - %d \n", radixSortWGS);
  fprintf(stderr, "globalCoopSizeRadixSort - %lu \n", globalCoopSizeRadixSort);
  
  /* Memobject info */
  fprintf(stderr, "inputArray size - %lu \n", getMemObjectSize(&inputArray));
  fprintf(stderr, "outputArray size - %lu \n", getMemObjectSize(&outputArray));
  fprintf(stderr, "globalCoopRadixSort size - %lu \n", getMemObjectSize(&globalCoopRadixSort));

  size_t totalSize = 0;
  for(int i = 0; i < numTypes; i++) {
    for(int j = 0; j < numLevels; j++) {
      totalSize += getMemObjectSize(&globalCoopPrefixSum[i][j]);
    }
  }
  fprintf(stderr, "Total GlobalCoopPrefixSum - %lu \n", totalSize);

  /* Print globalWorkSizes */
  fprintf(stderr, "radixSortGlobalWorkGroupSize - %lu %lu %lu \n", radixSortGlobalWorkGroupSize[0],
                                                                   radixSortGlobalWorkGroupSize[1],
                                                                   radixSortGlobalWorkGroupSize[2]);
  fprintf(stderr, "rearrangeKernelGlobalWorkGroupSize - %lu %lu %lu \n", 
                                                              rearrangeKernelGlobalWorkGroupSize[0],
                                                              rearrangeKernelGlobalWorkGroupSize[1],
                                                              rearrangeKernelGlobalWorkGroupSize[2]);  
  for(int i = 0; i <= numLevels; i++) {
    fprintf(stderr, "prefixSumKernel-level %d - WorkgroupSizes - %lu %lu %lu \n",
                                                              i,
                                                              prefixSumGlobalWorkGroupSize[i][0],
                                                              prefixSumGlobalWorkGroupSize[i][1],
                                                              prefixSumGlobalWorkGroupSize[i][2]);
  }
  for(int i = 0; i <numLevels; i++) {
    fprintf(stderr, "reductionKernel-leve %d - WorkgroupSizes - %lu %lu %lu \n",
                                                           i,
                                                           reductionKernelGlobalWorkGroupSize[i][0],
                                                           reductionKernelGlobalWorkGroupSize[i][1],
                                                           reductionKernelGlobalWorkGroupSize[i][2]);
                                                              
  }

#endif

  /* Enqueue kernels */
  /* Execute radixSort kernel */
  err = clEnqueueNDRangeKernel(*queue, sortKernel, 3, NULL, radixSortGlobalWorkGroupSize, localWorkSize, 0, NULL, NULL);
  assert(err == CL_SUCCESS);
  clFinish(*queue);
  fprintf(stderr, "Finished sort kernel \n");

  /* Execute prefixsum kernel */
  for(int i = 0; i < numTypes; i++) {
    for(int j = 0; j <= numLevels; j++) {
      err = clEnqueueNDRangeKernel(*queue, psKernel[i][j], 3, NULL, prefixSumGlobalWorkGroupSize[j], localWorkSize, 0, NULL, NULL);
      assert(err == CL_SUCCESS);

      clFinish(*queue);
      fprintf(stderr, "Finished prefix sum type - %d  \n", i);
    }
  }
  clFinish(*queue);
  fprintf(stderr, "Finished prefix sum \n");

  /* Execute reduction kernel in reverse order */
  for(int i = 0; i < numTypes; i++) {
    for(int j = numLevels - 1; j >= 0; j--) {
      err = clEnqueueNDRangeKernel(*queue, reductionKernel[i][j], 3, NULL, reductionKernelGlobalWorkGroupSize[j], localWorkSize, 0, NULL, NULL);
      assert(err == CL_SUCCESS);
    }
  }
  clFinish(*queue);
  fprintf(stderr, "Finished  reduction  \n");

  /* Execute final re-arrange kernel */
  if(inputSize > 512) {
    err = clEnqueueNDRangeKernel(*queue, rearrangeKernel, 3, NULL, rearrangeKernelGlobalWorkGroupSize, localWorkSize, 0, NULL, NULL);
    assert(err == CL_SUCCESS);
  }
  clFinish(*queue);
  fprintf(stderr, "Finished rearrange \n");

  /* Read data from GPU */
  err = clEnqueueReadBuffer(  *queue,
                              outputArray,
                              CL_TRUE,
                              0,
                              inputSize * sizeof(int),
                              gpuOutput,
                              0,
                              NULL,
                              NULL);
  assert(err == CL_SUCCESS);

  /* global coop radix sort */
  int cpuGlobalCoopRS[globalCoopSizeRadixSort];
  err = clEnqueueReadBuffer(  *queue,
                              globalCoopRadixSort,
                              CL_TRUE,
                              0,
                              globalCoopSizeRadixSort * sizeof(int),
                              cpuGlobalCoopRS,
                              0,
                              NULL,
                              NULL);
  assert(err == CL_SUCCESS);

  for(int i = 0; i < numTypes; i++) {
    for(int j = 0; j < radixSortWGS; j++) {
      fprintf(stderr, " %d - ", cpuGlobalCoopRS[i*radixSortWGS + j]); 
    }
    fprintf(stderr, " \n ");
  }
  fprintf(stderr, "Last index - %d \n", cpuGlobalCoopRS[globalCoopSizeRadixSort - 1]);

  /* Check output correctness */
  for(int i = 0; i < inputSize; i++) {
    if(cpuOutput[i] != gpuOutput[i]) {
      fprintf(stderr, "Incorrect output @ %d \n", i);
      fprintf(stderr, "index - correct order - incorrect order \n");
      for(int j = 0; j < inputSize; j++) {
        fprintf(stderr, "%d - %d - %d \n", j, cpuOutput[j], gpuOutput[j]);
      }
      retval = 0;
      break;
    }
  }

  /* Release host memory */
  free(hostInputArray);
  free(gpuOutput);

  /* Releasing device memory */
  clReleaseMemObject(inputArray);
  clReleaseMemObject(outputArray);

  for(int i = 0; i < numTypes; i++) {
    for(int j = 0; j < numLevels; j++) {
      clReleaseMemObject(globalCoopPrefixSum[i][j]);
    }
  }

  clReleaseMemObject(globalCoopRadixSort);

  /* Releasing kernels */
  clReleaseKernel(sortKernel);
  clReleaseKernel(rearrangeKernel);
  for(int i = 0; i < numTypes; i++) {
    for(int j = 0; j <=numLevels; j++) {
      clReleaseKernel(psKernel[i][j]);
      if(j < numLevels) {
        clReleaseKernel(reductionKernel[i][j]);
      }
    }
  }

  return retval;
}

int main(int argc, char *argv[]) {


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
  err |= clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
  assert(err == CL_SUCCESS);

  int dataAlignment = 0;
  err = clGetDeviceInfo(device_id,
                        CL_DEVICE_MEM_BASE_ADDR_ALIGN,
                        sizeof(int),
                        &dataAlignment,
                        NULL);
  assert(err == CL_SUCCESS);
  
  fprintf(stderr, "Dataalignemnt - %d \n", dataAlignment);

  // Create a context 
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  assert(err == CL_SUCCESS);
  
  // Create a command queue
  queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);
  assert(err == CL_SUCCESS);
  
  /* Get program from file */
  char path[100] = "/home/varun/programming/OpenCL/radixSortKernel.cl";
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

  doRandRadixSort(INPUTSIZE, &context, &program, &queue);

  free(kernelSource);

  /* Release program, context and command queue */
  clReleaseProgram(program);
  clReleaseContext(context);
  clReleaseCommandQueue(queue);

  return 0;
}

