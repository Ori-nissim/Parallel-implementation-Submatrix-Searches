#include "header.h"
#include <cuda_runtime.h>
#include <helper_cuda.h>

// num of threads will be 3840 (num of cuda cores in our GPU) , each block will contain 32 threads, num of block will be 3840 / warp size (32)
int allocateDataOnDevice(Picture* device_Pics,Object* device_Objs,InputData* inputDataPtr)
{
    cudaError_t err = cudaSuccess;
    
	// Allocate and copy pictures
	int size = inputDataPtr->num_pictures * sizeof(Picture);
	err = cudaMalloc((void **)&device_Pics, size);

	if (err != cudaSuccess) {
		fprintf(stderr, "Failed to allocate device memory (error code %s)!\n",
	    cudaGetErrorString(err));
		exit(EXIT_FAILURE);
		return 0;
	  }

	// copy data to device
	printf("Copying pictures from the host memory to the CUDA device\n");
	err = cudaMemcpy(device_Pics, inputDataPtr->pictures, size, cudaMemcpyHostToDevice);

	if (err != cudaSuccess) {
	fprintf(stderr, "Failed to copy from host to device (error code %s)!\n",
    cudaGetErrorString(err));
	exit(EXIT_FAILURE);
	return 0;
  }
  
  // Allocate and copy Objects
	size = inputDataPtr->num_objects * sizeof(Object);
	err = cudaMalloc((void **)&device_Objs, size);

	if (err != cudaSuccess) {
		fprintf(stderr, "Failed to allocate device memory (error code %s)!\n",
	    cudaGetErrorString(err));
		exit(EXIT_FAILURE);
		return 0;
	  }

	// copy data to device
	printf("Copying objects from the host memory to the CUDA device\n");
	err = cudaMemcpy(device_Pics, inputDataPtr->objects, size, cudaMemcpyHostToDevice);

	if (err != cudaSuccess) {
	fprintf(stderr, "Failed to copy from host to device (error code %s)!\n",
    cudaGetErrorString(err));
	exit(EXIT_FAILURE);
	return 0;
  }
  
  return 1; // success
 	 
}
