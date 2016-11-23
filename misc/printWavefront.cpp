#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define WAVEFRONT_SIZE 64

bool wavefrontMapping_algoCheck(int globalDimX, int globalDimY, int localDimX, int localDimY, int *input) {

  int numWFRows = WAVEFRONT_SIZE / localDimX;
  int *val = (int *)calloc(numWFRows, sizeof(int));

  /* num wavefront rows -- that many vals */
  for(int i = 0; i < numWFRows; i++) {
    val[i] = i * localDimX;
  }

  for(int i = 0; i < globalDimY; i++) {
    for(int j = 0; j < globalDimX; j++) {
      input[i * globalDimX + j] = val[i%numWFRows] + ((i * globalDimX) + j) % localDimX;
    }
  }

  free(val);
  return true;
}

bool wavefrontMapping_algoCheck_opCheck(int globalDimX, int globalDimY, int *op) {
  for(int i = 0; i < globalDimY; i++) {
    for(int j = 0; j < globalDimX; j++) {
      if(op[i * globalDimX + j] != (i * globalDimX + j)%WAVEFRONT_SIZE) { 
        fprintf(stderr, "Assertion error becos of index - %d\n", i * globalDimX + j);
        return false;
      }
    }
  }
 return true; 
}

bool dumpActiveThreads_WavefrontBased(int *op, int *activeThreads, int globalDimX, int globalDimY, int localDimX, int localDimY) {
 
  /* We need local work size to exactly fit into global work size
   * That is, make sure that the tile dim is a multiple of local dim
   */
  assert((globalDimX%localDimX) == 0);
  assert((globalDimY%localDimY) == 0);

  int numWGSX = globalDimX/localDimX;
  int numWGSY = globalDimY/localDimY;
  int numWFPerWG = (localDimX * localDimY)/WAVEFRONT_SIZE;
  int globalStride = globalDimX;
  int localStride = localDimX;

  /* We need our wavefronts to fit into complete matrices */
  assert((WAVEFRONT_SIZE%localStride) == 0);

  int opX = 0;
  int opY = 0;

  for(int i = 0; i < numWGSY; i++) {
    for(int j = 0; j < numWGSX; j++) {
      int currentWGOffset = i * (globalStride * localDimY) + j * localStride;
      for(int k = 0; k < numWFPerWG; k++) {
        int wavefrontOffset = k * (WAVEFRONT_SIZE/localStride);
        /* print wave front */
        for(int m = wavefrontOffset; m < wavefrontOffset + WAVEFRONT_SIZE/localStride; m++) {
          for(int l = 0; l < localStride; l++) { 
            int index =  currentWGOffset + (m * globalStride) + l;

            if(opX == globalStride) { opX = 0; opY++; }
            int opIndex = opY * globalStride + opX; 

            op[opIndex] = activeThreads[index];

            opX++;
          }
        }
      }
    }
  }
}

int main() {

  int globalDimX = 1024;
  int globalDimY = 1024;
  int localDimX = 8;
  int localDimY = 8;

  int *activeThreads = (int *)calloc(globalDimX * globalDimY, sizeof(int));
  int *op = (int *)calloc(globalDimX * globalDimY, sizeof(int));

  wavefrontMapping_algoCheck(globalDimX, globalDimY, localDimX, localDimY, activeThreads);


  dumpActiveThreads_WavefrontBased(op, activeThreads, globalDimX, globalDimY, localDimX, localDimY);

#if 0
  for(int i = 0; i < globalDimY; i++) {
    for(int j = 0; j < globalDimX; j++) {
      fprintf(stderr, " %d", activeThreads[i * globalDimX + j]);
    }
    fprintf(stderr, "\n");
  }

  for(int i = 0; i < globalDimY; i++) {
    for(int j = 0; j < globalDimX; j++) {
      fprintf(stderr, " %d", op[i * globalDimX + j]);
    }
    fprintf(stderr, "\n");
  }
#endif

  assert(wavefrontMapping_algoCheck_opCheck(globalDimX, globalDimY, op));

  free(activeThreads);
  free(op);
}
