#include "header.h"

int main(int argc, char *argv[]) {

		int result;
		int rank, size;

	  // Initialize MPI
	  MPI_Init(&argc, &argv);
	  
	  // Get the rank of the current process and the total number of processes
	  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	  MPI_Comm_size(MPI_COMM_WORLD, &size);
	  

	 
// ############## RANK 0 ######################
	  if (rank == 0) {
	  
	    // Start measuring time
	    clock_t start = clock();

	  // read input file 
		struct InputData* inputDataPtr = (struct InputData*)malloc(sizeof(struct InputData));
		CHECK_MALLOC(inputDataPtr);
		readInputFile("input.txt",inputDataPtr);
		int numPics = inputDataPtr->num_pictures,numObjs =inputDataPtr->num_objects;
		

		// Send all necessary data to process 1
		MPIsendData(numObjs, inputDataPtr->objects, 1, inputDataPtr->matching_value, numPics);
		int half = numPics % 2 == 0 ?  numPics/2 : numPics/2 + 1;
		MPI_Send(&half ,1 , MPI_INT, 1,0 ,MPI_COMM_WORLD);  // send amount
		MPISendHalfPictures(inputDataPtr->pictures, half );
		
		
		 // matrix to store results that will be written later to the output file, see structure in the header file
	  	int** resultMatrix = (int**)malloc(sizeof(int*)* numPics);
	  	CHECK_MALLOC(resultMatrix);
	
	  // allocate memory for results matrix - It will store the results during the search
	  for (int i = 0; i < numPics; i++) 
	  {
		  resultMatrix[i] = (int *)malloc(RESULT_COLUMN * sizeof(int));
		  CHECK_MALLOC(resultMatrix[i]);
	  }
	  
  	 // TODO- Remove the num of threads when running on 2 different machines
	#pragma omp parallel for num_threads(2)
	for (int i = half ; i < numPics; i ++ ) 
	{
		SearchObjectsInPicture( resultMatrix[i],inputDataPtr->pictures[i],inputDataPtr->objects,numObjs, inputDataPtr->matching_value, rank); //  search all objects inside the image
	}
	
	// get results from the other process
	getResults(resultMatrix, half);
	writeOutputFile(resultMatrix, numPics, RESULT_COLUMN);

   // Stop measuring time
    clock_t end = clock();

	// Calculate elapsed time in seconds
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", time);

	// free memory
	freeResultMatrix(resultMatrix, numPics);

  }
	  
// ############## RANK 1 ######################
	 if (rank == 1) {
	  	int numObjs, numPics, JobsNotDone = 1;
	  	float matching_value;
		Object* objects = MPIrecvData(0, &numObjs, &matching_value, &numPics);
		
		printf("Received %d objects, matching values is %f, num pics is %d\n",numObjs, matching_value, numPics);
		
		
		 // matrix to store results that will be written later to the output file, see structure in the header file
	  	int** resultMatrix = (int**)malloc(sizeof(int*)* numPics);
	  	CHECK_MALLOC(resultMatrix);
	
	  // allocate memory for results matrix - It will store the results during the search
	  for (int i = 0; i < numPics; i++) 
	  {
		  resultMatrix[i] = (int *) malloc(RESULT_COLUMN * sizeof(int));
		  CHECK_MALLOC(resultMatrix[i]);
	  }
	  
	  // Make sure we have space for the coming pictures
	  int amount;
      MPI_Recv(&amount ,1 , MPI_INT, 0,MPI_ANY_TAG ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // get amount of pics
	  Picture* allPictures = (Picture*)malloc(sizeof(Picture) * amount);
  	  CHECK_MALLOC(allPictures);
	  MPIRecvHalfPictures(allPictures, amount);	


	 // TODO- Remove the num of threads when running on 2 different machines
	#pragma omp parallel for num_threads(2)
	for (int i = 0 ; i < amount; i ++ ) 
	{	
		SearchObjectsInPicture( resultMatrix[i],allPictures[i],objects,numObjs, matching_value, rank); //  search all objects inside the image
			 
	}
	
	 sendResults(resultMatrix, amount);
	  
	freeResultMatrix(resultMatrix, numPics);

	}
	 
	  // Finalize MPI
	  MPI_Finalize();
  


	return 0;
}


