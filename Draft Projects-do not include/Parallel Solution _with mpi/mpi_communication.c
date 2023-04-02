#include "header.h"
void save_pixels(Object *obj, int *pixels_array) {
    int i, j;
    for (i = 0; i < obj->size; i++) {
        for (j = 0; j < obj->size; j++) {
            *pixels_array++ = obj->pixels[i][j];
        }
    }
}
void savePixelsAs2DArray(int* pixels1D, int size, Object* obj) {
    // Iterate over each row in the 2D array
    for (int i = 0; i < obj->size; i++) {
        // Iterate over each column in the current row
        for (int j = 0; j < obj->size; j++) {
            // Compute the 1D index for the current pixel
            int index = i * obj->size + j;
            // Save the pixel in the corresponding location in the 2D array
            obj->pixels[i][j] = pixels1D[index];
        }
    }
}

void save_pixels_Picture(Picture* pic, int *pixels_array) {
    int i, j;
    for (i = 0; i < pic->size; i++) {
        for (j = 0; j < pic->size; j++) {
            *pixels_array++ = pic->pixels[i][j];
        }
    }
}
void savePixelsAs2DArray_Picture(int* pixels1D, int size, Picture* pic) {
    // Iterate over each row in the 2D array
    for (int i = 0; i < pic->size; i++) {
        // Iterate over each column in the current row
        for (int j = 0; j < pic->size; j++) {
            // Compute the 1D index for the current pixel
            int index = i * pic->size + j;
            // Save the pixel in the corresponding location in the 2D array
            pic->pixels[i][j] = pixels1D[index];
        }
    }
}
void printPixels(int *pixels, int size) {
printf("process 1:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", pixels[i]);
    }
    printf("\n");
}


void MPIsendData(int numObjs, Object* objects, int destinationRank,float matching_value, int numPics)
{
	// send number of objects
	MPI_Send(&numObjs ,1 , MPI_INT, 1,0 ,MPI_COMM_WORLD); 
	int* pixels = (int*)malloc(sizeof(int) * MAX_OBJECT_SIZE * MAX_OBJECT_SIZE); // convert to 1D array
		// check malloc
	for (int i =0; i < numObjs; i++) 
	{
		save_pixels(&objects[i],pixels);
		int objSize = objects[i].size;
		MPI_Send(&objects[i].id ,1 , MPI_INT, 1,0 ,MPI_COMM_WORLD);  // send id
		MPI_Send(&objSize ,1 , MPI_INT, 1,0 ,MPI_COMM_WORLD); // send size
		MPI_Send(pixels , objSize* objSize , MPI_INT, 1,0 ,MPI_COMM_WORLD); // send pixels
		printf("Sent object #%d\n", i+1);
	}
	
	MPI_Send(&matching_value ,1 , MPI_INT, 1,0 ,MPI_COMM_WORLD); 
	MPI_Send(&numPics ,1 , MPI_INT, 1,0 ,MPI_COMM_WORLD); 
	
	free(pixels);
}


Object* MPIrecvData(int sourceRank,int* numObjs, float* matching_value, int* numPics)
{
	
	MPI_Recv(numObjs ,1 , MPI_INT, 0,0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);  
	int* pixels = (int*)malloc(sizeof(int) * MAX_OBJECT_SIZE * MAX_OBJECT_SIZE); //  1D array, will be later converted to 2D
	Object* objects = (Object*)malloc(sizeof(Object)* (*numObjs));
	printf("recv num objects %d\n", *numObjs);
	int objSize;
	for (int i =0; i < *numObjs; i++) 
	{
		MPI_Recv(&objects[i].id ,1 , MPI_INT, 0,0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);  
		MPI_Recv(&objSize ,1 , MPI_INT, 0,0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
		objects[i].size=objSize;
		MPI_Recv(pixels , objSize* objSize , MPI_INT, 0,0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
		savePixelsAs2DArray(pixels, objects[i].size,&objects[i]);
		printf("recv object #%d\n", i+1);
	}
	free(pixels);
	MPI_Recv(matching_value ,1 , MPI_INT, 0,0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);  
	MPI_Recv(numPics ,1 , MPI_INT, 0,0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  
	return objects;
}

void sendJob(Picture picture, int tag) // tag = 0 -- there will be more jobs, tag = 1 --- this is the last job
{
	int picSize = picture.size;
	int* pixels = (int*)malloc(sizeof(int) * MAX_PICTURE_SIZE * MAX_PICTURE_SIZE); // convert to 1D array
		// check malloc
	save_pixels_Picture(&picture, pixels);
	MPI_Send(&picture.id ,1 , MPI_INT, 1,tag ,MPI_COMM_WORLD);  // send id
	MPI_Send(&picSize ,1 , MPI_INT, 1,tag ,MPI_COMM_WORLD); // send size
	MPI_Send(pixels , picSize* picSize , MPI_INT, 1,tag ,MPI_COMM_WORLD); // send pixels
	printf("Sent pic #%d, size %d\n", picture.id, picSize);
	free(pixels);
}


void recvJob(Picture* picture, int* tag) // tag = 0 -- there will be more jobs, tag = 1 --- this is the last job
{
	printf("Entered function\n");
	int picSize;
	MPI_Status status;
	int* pixels = (int*)malloc(sizeof(int) * MAX_PICTURE_SIZE * MAX_PICTURE_SIZE); // convert to 1D array
		// check malloc
		
	MPI_Recv(&picture->id ,1 , MPI_INT, 0,MPI_ANY_TAG ,MPI_COMM_WORLD, &status);  // id
	MPI_Recv(&picSize ,1 , MPI_INT, 0,MPI_ANY_TAG,MPI_COMM_WORLD, &status);  // size
	picture->size = picSize;
	
	MPI_Recv(pixels , picSize* picSize , MPI_INT, 0,MPI_ANY_TAG ,MPI_COMM_WORLD, &status); // pixels
		//printPixels(pixels, picSize * picSize);
	savePixelsAs2DArray_Picture(pixels, picSize, picture);
	*tag = status.MPI_TAG;
	printf("recv pic #%dsize %d\n", picture->id, picSize);
	free(pixels);
}

