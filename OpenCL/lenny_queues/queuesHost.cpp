#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <CL/opencl.h>


char * getKernelSource(char *path) {

  FILE *fp = fopen(path, "r");
  assert(fp != NULL);
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

int main(int argc, char *argv[]) {

  /* Arg 1 - numQueues
   * Arg 2 - numGlobalThreads
   */

  if(argc != 3) {
    fprintf(stderr, "Insufficient arguments ");
    return 0;
  }

  int numQueue = atoi(argv[1]);
  int numGlobalThreads = atoi(argv[2]);

  srand(time(NULL));

  cl_platform_id cpPlatform;        // OpenCL platform
  cl_device_id device_id;           // device ID
  cl_context context;               // context
  cl_command_queue comm_queue;           // command queue
  cl_program program;               // program
  
  cl_int err = CL_SUCCESS;
  
  // Bind to platform
  err |= clGetPlatformIDs(1, &cpPlatform, NULL);
  
  // Get ID for the device
  err |= clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
  assert(err == CL_SUCCESS);
  
  // Create a context 
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  assert(err == CL_SUCCESS);
  
  // Create a command queue
  comm_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);
  assert(err == CL_SUCCESS);
  
  /* Get program from file */
  char path[100] = "/home/varun/programming/OpenCL/lenny_queues/queue_demo.cl";
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

  cl_kernel queueKernel = clCreateKernel(program,
                                         "Queue",
                                         &err);
  assert(err == CL_SUCCESS);

  /* Create buffers for queues */
  cl_mem queues_1 = clCreateBuffer(context,
                                 CL_MEM_READ_WRITE,
                                 numGlobalThreads * sizeof(int),
                                 NULL,
                                 &err);
  assert(err == CL_SUCCESS);

  cl_mem queues_2 = clCreateBuffer(context,
                                 CL_MEM_READ_WRITE,
                                 numGlobalThreads * sizeof(int),
                                 NULL,
                                 &err);
  assert(err == CL_SUCCESS);

  cl_mem withinQueueAtomics = clCreateBuffer(context,
                                             CL_MEM_READ_WRITE,
                                             numQueue * sizeof(unsigned int),
                                             NULL,
                                             &err);
  assert(err == CL_SUCCESS);

  int *hostWithinQueueAtomics = (int *)calloc(numQueue, sizeof(unsigned int));

  err = clEnqueueWriteBuffer(comm_queue,
                             withinQueueAtomics,
                             CL_TRUE,
                             0,
                             numQueue * sizeof(unsigned int),
                             hostWithinQueueAtomics,
                             0,
                             NULL,
                             NULL);
  assert(err == CL_SUCCESS);

  int *hostQueues = (int *)calloc( sizeof(int), numGlobalThreads);

  err = clEnqueueWriteBuffer(comm_queue,
                             queues_1,
                             CL_TRUE,
                             0,
                             numGlobalThreads * sizeof(int),
                             hostQueues,
                             0,
                             NULL,
                             NULL);
  assert(err == CL_SUCCESS);

  err = clEnqueueWriteBuffer(comm_queue,
                             queues_2,
                             CL_TRUE,
                             0,
                             numGlobalThreads * sizeof(int),
                             hostQueues,
                             0,
                             NULL,
                             NULL);
  assert(err == CL_SUCCESS);

  size_t globalSize[1] = {(size_t)numGlobalThreads};
  size_t localSize[1] = {256};
  /* Set kernel arguments */
  err = CL_SUCCESS;
  err |= clSetKernelArg(queueKernel,
                        0,
                        sizeof(cl_mem),
                        &queues_1);
  err |= clSetKernelArg(queueKernel,
                        1,
                        sizeof(cl_mem),
                        &queues_2);
  err |= clSetKernelArg(queueKernel,
                        2,
                        sizeof(cl_mem),
                        &withinQueueAtomics);
  assert(err == CL_SUCCESS);

  err = clEnqueueNDRangeKernel( comm_queue,
                                queueKernel,
                                1,
                                NULL,
                                globalSize,
                                localSize,
                                0,
                                NULL,
                                NULL);

  clFinish(comm_queue);

  /* Check output */
  err = clEnqueueReadBuffer(comm_queue,
                            queues_1,
                            CL_TRUE,
                            0,
                            numGlobalThreads * sizeof(int),
                            hostQueues,
                            0,
                            NULL,
                            NULL);
  assert(err == CL_SUCCESS);

  for(int i = 0; i < numGlobalThreads; i++) {
    assert(hostQueues[i] % 2 == 0 || hostQueues[i] == 0);
  }

  err = clEnqueueReadBuffer(comm_queue,
                            queues_2,
                            CL_TRUE,
                            0,
                            numGlobalThreads * sizeof(int),
                            hostQueues,
                            0,
                            NULL,
                            NULL);
  assert(err == CL_SUCCESS);

  for(int i = 0; i < numGlobalThreads; i++) {
    assert(hostQueues[i] % 2 == 1 || hostQueues[i] == 0);
  }

  free(kernelSource);
  free(hostWithinQueueAtomics);
  free(hostQueues);
  return 0;
}

