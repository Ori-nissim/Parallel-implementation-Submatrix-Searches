#include "header.h"

#define WARP_SIZE 32
#define NUM_THREADS 3840

// num of threads will be 3840 (num of cuda cores in our GPU) , each block will contain 32 threads, num of block will be 3840 / warp size (32)
int main(int argc, char *argv[]) {

		int result;
		// for time measurement
		clock_t start, end;
		double time;
		 
		start = clock();
		
		// read input file 
		struct InputData* inputDataPtr = (struct InputData*)malloc(sizeof(struct InputData));
		CHECK_MALLOC(inputDataPtr);
		readInputFile("input.txt",inputDataPtr);
		int numPics = inputDataPtr->num_pictures,numObjs =inputDataPtr->num_objects;
		
		end = clock();
		time = ((double) (end - start)) / CLOCKS_PER_SEC;
	   

		printf("Input file read Successfully! containing %d pictures and %d objects\n\tCompleted in: %f ms\n", numPics, numObjs,time * 1000);
	
		
		// matrix to store results that will be written later to the output file, see structure in the header file
		int** resultMatrix = (int**)malloc(sizeof(int*)* numPics);
		CHECK_MALLOC(resultMatrix);
		
		// allocate memory for results matrix - It will store the results during the search
		for (int i = 0; i < numPics; i++) 
		{
			resultMatrix[i] = (int *) malloc(RESULT_COLUMN * sizeof(int));
			CHECK_MALLOC(resultMatrix[i]);
		}
		
		//We will need access to pictures and objects during the search
	    Picture* device_Pics = NULL;
        Object* device_Objs = NULL;
		result = allocateDataOnDevice(device_Pics,device_Objs,inputDataPtr);
		if (result == SUCCESS) printf("Pictures & Objects copied successfully to GPU memory\n");
		
		//Start the search
		start = clock();	
		#pragma omp parallel for
		for (int i= 0; i < numPics; i ++)
			//SearchObjectsInPicture( resultMatrix[i],inputDataPtr->pictures[i],inputDataPtr->objects,numObjs, inputDataPtr->matching_value); // for each image, search all objects inside it
		
		for (int i=0 ; i < numPics ; i ++)
				printf("Pic #%d: found %d objects\n", i+1, resultMatrix[i][0]);
			
			
		end = clock();
		time = ((double) (end - start)) / CLOCKS_PER_SEC;
	   

		printf("Finished Searching Successfully! \n\tCompleted in: %f s\n", time);
	

	// send half of the images to the other process
	// search for the objects in remaining pictures & save results
	// get results from the other process
	// free memory on process 1
	// save to file
	// free memory on procees 0
	
}



