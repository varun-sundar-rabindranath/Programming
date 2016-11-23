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

#define DEBUG 1

using namespace std;

size_t localWorkSize[3] = {WGS, 1, 1};

bool compFunction(int i, int j) {
  return i<j;
}

void printMatrix(
                  int rows,
                  int cols,
                  int *inputArray) {
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      fprintf(stderr, " %d ", inputArray[i * cols + j]);
    }
    fprintf(stderr, "\n");
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


bool doTranspose( int rows,
                  int cols,
                  cl_context *context,
                  cl_program *program,
                  cl_command_queue *queue) {

  bool retval = 1;
  cl_int err = CL_SUCCESS;

  size_t inputSize = rows * cols;

  /* Declaring host memory */
  int *hostInputArray = NULL;
  int *hostOutputArray = NULL;
  int *gpuOutput = NULL;

  /* Creating host input */
  hostInputArray = (int *)malloc(inputSize * sizeof(int));
  assert(hostInputArray != NULL);

  /* Creating host output array */
  hostOutputArray = (int *) malloc(inputSize * sizeof(int));
  assert(hostOutputArray != NULL);

  /* Creating output array to store GPU computed results */
  gpuOutput = (int *)malloc(inputSize * sizeof(int));
  assert(gpuOutput != NULL);

  // Filling host input array with random values
  for(int i = 0; i < inputSize; i++) {
    hostInputArray[i] = i;
  }

  /* Compute output using cpu */
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      hostOutputArray[j * rows + i] = hostInputArray[i* cols + j];
    }
  }

  /* Declaring events and kernels */
  cl_kernel transposeKernel;

  /* Declaring all cl_mem(s) */
  cl_mem inputArray, outputArray;

  /* Create memory */
  inputArray = clCreateBuffer(*context,
                              CL_MEM_WRITE_ONLY,
                              inputSize * sizeof(int),
                              NULL,
                              &err);
  assert(err == CL_SUCCESS);

  outputArray = clCreateBuffer(*context,
                              CL_MEM_READ_ONLY,
                              inputSize * sizeof(int),
                              NULL,
                              &err);
  assert(err == CL_SUCCESS);

  /* Create radix sort kernel */
  transposeKernel = clCreateKernel( *program, "transposeKernel", &err); 
  assert(err == CL_SUCCESS);

  /* set radix sort kernel arguments */

  err |= clSetKernelArg( transposeKernel, 0, sizeof(cl_mem), &inputArray);
  err |= clSetKernelArg( transposeKernel, 1, sizeof(cl_mem), &outputArray);
  err |= clSetKernelArg( transposeKernel, 2, sizeof(int), &rows);
  err |= clSetKernelArg( transposeKernel, 3, sizeof(int), &cols);
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
  size_t transposeKernelGlobalWorkSize[3] = {(size_t)(((cols - 1) / 16) + 1) * 16,
                                             (size_t) (((rows - 1) / 16) + 1) * 16,
                                              1};
  size_t transposeKernelLocalWorkSize[3] = {16, 16, 1};
  
  fprintf(stderr, "Global work size - %lu %lu %lu \n", transposeKernelGlobalWorkSize[0],
                                                    transposeKernelGlobalWorkSize[1],
                                                    transposeKernelGlobalWorkSize[2]);
  /* Run transpose kernel */
  err = clEnqueueNDRangeKernel( *queue, transposeKernel, 3, NULL, transposeKernelGlobalWorkSize, transposeKernelLocalWorkSize, 0, NULL, NULL);
  assert(err == CL_SUCCESS);

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

  /* Check output correctness */
  for(int i = 0; i < inputSize; i++) {
    if(hostOutputArray[i] != gpuOutput[i]) {
      fprintf(stderr, "Incorrect output \n");
      printMatrix(rows, cols, hostInputArray);
      printMatrix(cols, rows, hostOutputArray);
      printMatrix(cols, rows, gpuOutput);
      break;
    }
  }

  /* Release host memory */
  free(hostInputArray);
  free(hostOutputArray);
  free(gpuOutput);

  /* Releasing device memory */
  clReleaseMemObject(inputArray);
  clReleaseMemObject(outputArray);

  /* Release kernel */
  clReleaseKernel(transposeKernel);

  return retval;
}

int main(int argc, char *argv[]) {


  srand(time(NULL));

  int rows = atoi(argv[1]);
  int cols = atoi(argv[2]);

 // fprintf(stderr, "Rows - %d | Cols - %d \n", rows, cols);

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
  
  //fprintf(stderr, "Dataalignemnt - %d \n", dataAlignment);

  // Create a context 
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  assert(err == CL_SUCCESS);
  
  // Create a command queue
  queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);
  assert(err == CL_SUCCESS);
  
  /* Get program from file */
  char path[100] = "/home/varun/programming/OpenCL/transpose/trasposeKernel.cl";
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
  doTranspose(rows, cols, &context, &program, &queue);

  free(kernelSource);

  /* Release program, context and command queue */
  clReleaseProgram(program);
  clReleaseContext(context);
  clReleaseCommandQueue(queue);

  return 0;
}

