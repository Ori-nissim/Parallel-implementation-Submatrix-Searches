#include "header.h"

void printPicture(Picture pic)
{
	printf("Pic ID: %d, Pic size: %d\n", pic.id, pic.size);
	
	for (int i = 0; i < pic.size ; i ++)
	{
			for (int j = 0; j < pic.size ; j ++)
		{
			printf("%d ",pic.pixels[i][j]);
		}
		printf("\n");
	}
	
}


void printObject(Object obj)
{
	printf("Object ID: %d, Object size: %d\n", obj.id, obj.size);
	
	for (int i = 0; i < obj.size ; i ++)
	{
			for (int j = 0; j < obj.size ; j ++)
		{
			printf("%d ",obj.pixels[i][j]);
		}
		printf("\n");
	}
	

}

void sortPicturesBySize(Picture pictures[], int numPictures) {
    int i, j;
    Picture temp;
    for (i = 0; i < numPictures; i++) {
        for (j = i+1; j < numPictures; j++) {
            if (pictures[j].size > pictures[i].size) {
                temp = pictures[i];
                pictures[i] = pictures[j];
                pictures[j] = temp;
            }
        }
    }
}


int main(int argc, char *argv[]) {

		int result;
		// for time measurement
		clock_t start, end;
		double time;
		 
		int rank, size, tag = 0;
 	   char message[100];
	  MPI_Status status;

	  // Initialize MPI
	  MPI_Init(&argc, &argv);
	  
	  // Get the rank of the current process and the total number of processes
	  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	  MPI_Comm_size(MPI_COMM_WORLD, &size);
	  

	 
// ############## RANK 0 ######################
	  if (rank == 0) {
	  
	  // read input file 
		struct InputData* inputDataPtr = (struct InputData*)malloc(sizeof(struct InputData));
		CHECK_MALLOC(inputDataPtr);
		readInputFile("input.txt",inputDataPtr);
		int numPics = inputDataPtr->num_pictures,numObjs =inputDataPtr->num_objects;
		
		sortPicturesBySize(inputDataPtr->pictures, numPics);
		
		MPIsendData(numObjs, inputDataPtr->objects, 1, inputDataPtr->matching_value, numPics);
		
		 // matrix to store results that will be written later to the output file, see structure in the header file
	  	int** resultMatrix = (int**)malloc(sizeof(int*)* numPics);
	  	CHECK_MALLOC(resultMatrix);
	
	  // allocate memory for results matrix - It will store the results during the search
	  for (int i = 0; i < numPics; i++) 
	  {
		  resultMatrix[i] = (int *) malloc(RESULT_COLUMN * sizeof(int));
		  CHECK_MALLOC(resultMatrix[i]);
	  }
	
		// create a dynamic task pool
		int currentJob = 0, tag = 0;

		while (currentJob < numPics)
		{
			if ( currentJob == (numPics-1) || currentJob == (numPics-2) ) tag =1; // if this is the last job, send tag = 1
					printf("entering send jobs\n");
			sendJob(inputDataPtr->pictures[currentJob++], tag );
			SearchObjectsInPicture( resultMatrix[currentJob],inputDataPtr->pictures[currentJob],inputDataPtr->objects,numObjs, inputDataPtr->matching_value, rank); //  search all objects inside the image
			currentJob++;
		
		}
		
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
	  
	  // search one picture at a time
	  Picture* picture = (Picture*)malloc(sizeof(Picture)); 
	  CHECK_MALLOC(picture);
	  		printf("Starting jobs\n");
	  while (JobsNotDone)
	  {
	  		printf("recving jobs jobs\n");
	  	recvJob(picture,&tag);
	  	printf("picture id: %d, size: %d, numobjs %d, matching: %f\n", picture->id, picture->size, numObjs, matching_value);
	  	//printPicture(*picture);
		SearchObjectsInPicture( resultMatrix[picture->id-1],*picture,objects,numObjs, matching_value,rank); //  search all objects inside the image
		
	  	if (tag == 1) JobsNotDone = 0;
	  		
	  }
	  
	}
	  
	  // Finalize MPI
	  MPI_Finalize();
  

	
	// get results from the other process
	// free memory on process 1
	// save to file
	// free memory on procees 0
	return 0;
}


