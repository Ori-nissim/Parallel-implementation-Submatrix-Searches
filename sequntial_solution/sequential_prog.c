// objects starts at 1363
#include "header.h"

//############# MAIN FUNCTION #####################
int main (int argc, char *argv[]) 
{
	    // Start measuring time
	    clock_t start = clock();

	  // read input file 
		struct InputData* inputDataPtr = (struct InputData*)malloc(sizeof(struct InputData));
		CHECK_MALLOC(inputDataPtr);
		readInputFile("input.txt",inputDataPtr);
		int numPics = inputDataPtr->num_pictures,numObjs =inputDataPtr->num_objects;
		

		 // matrix to store results that will be written later to the output file, see structure in the header file
	  	int** resultMatrix = (int**)malloc(sizeof(int*)* numPics);
	  	CHECK_MALLOC(resultMatrix);
	
	  // allocate memory for results matrix - It will store the results during the search
	  for (int i = 0; i < numPics; i++) 
	  {
		  resultMatrix[i] = (int *)malloc(RESULT_COLUMN * sizeof(int));
		  CHECK_MALLOC(resultMatrix[i]);
	  }
	  
  	
	for (int i = 0 ; i < numPics; i ++ ) 
	{
		SearchObjectsInPicture( resultMatrix[i],inputDataPtr->pictures[i],inputDataPtr->objects,numObjs, inputDataPtr->matching_value); //  search all objects inside the image
	}
	

	writeOutputFile(resultMatrix, numPics, RESULT_COLUMN);

   // Stop measuring time
    clock_t end = clock();

	// Calculate elapsed time in seconds
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", time);

	
	
}	


//############ Functions Implemetations #####################

int checkObjectInPosition(Picture pic,Object obj,int row,int column, float matching_value)
{
	//printf("Search in position %d,%d\n",row,column);
	float difference  = 0.0, found;
	for (int i = row, k=0; i < row + obj.size ; i ++,k++)
	{
		for (int j = column,l=0; j < column + obj.size; j++,l++)
		{
			//diff = abs((p – o)/p)
			difference += fabs(((pic.pixels[i][j] - obj.pixels[k][l]) / (pic.pixels[i][j] )));
			
		}
	}
	difference /= obj.size * obj.size;

	found = difference <= matching_value ? 1 : 0;
	return found;

}
// TODO - make sure the search stops after finding an object and continue to the next one
// This function gets a picture and the array of objects and searches for 3 objects in that picture
int SearchObjectsInPicture(int* resultRow,Picture pic, Object* objects,int numObjects, float matching_value)
{
	printf("Starting search at picture %d\n",pic.id);
	int foundObjectsArray[numObjects]; // will store 1 if the object was found, so we sill stop searching for it in the picture
	memset( foundObjectsArray, 0, numObjects*sizeof(int) );

	int objectsCounter = 0, objSize, objectHasBeenFound = 0;

	for (int i=0 ; i < pic.size; i ++)
		{
			for (int j=0; j < pic.size; j ++)
			{	
				for (int k =0 ; k < numObjects; k ++)
				{
					if (foundObjectsArray[k] == 1 ) continue; // check if object has been found, if so, continue to the next object
					// check picture borders limits
					objSize = objects[k].size; 
					if ( ( ( i + objSize) < pic.size ) && ( ( j + objSize) < pic.size ) )
					{
					
						objectHasBeenFound = checkObjectInPosition(pic,objects[k],i,j,matching_value);
						if (objectHasBeenFound) 
						{				
							printf("\t\tObject %d found in picture %d indices (%d,%d) \n", objects[k].id, pic.id, i,j);
							resultRow[1 + 3*objectsCounter] = objects[k].id;
							resultRow[1 + 3*objectsCounter+ 1] = i;
							resultRow[1 + 3*objectsCounter + 2] = j;
							objectsCounter++;
							foundObjectsArray[k] = 1;
							resultRow[0] = objectsCounter;
							if (objectsCounter == 3) return 1; // found 3 different objects, we can stop searching in this picture
						}
					}
				}
			}	
		}
	
	return 1;
	
}
int readInputFile(const char* filename, InputData* data)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file '%s'\n", filename);
        return 0;
    }
    // read matching value
    if (fscanf(fp, "%f", &data->matching_value) != 1) {
        fprintf(stderr, "Error: could not read matching value from file '%s'\n", filename);
        fclose(fp);
        return 0;
    }
    readPictures(fp,filename, data);
    readObjects(fp,filename, data);
 }
 
 int readPictures( FILE* fp,const char* filename ,InputData* data)
 {
    // read number of pictures
    if (fscanf(fp, "%d", &data->num_pictures) != 1) {
        fprintf(stderr, "Error: could not read number of pictures from file '%s'\n", filename);
        fclose(fp);
        return 0;
    }
    // allocate memory for pictures
    data->pictures = (Picture*) malloc(data->num_pictures * sizeof(Picture));
    if (data->pictures == NULL) {
        fprintf(stderr, "Error: could not allocate memory for pictures\n");
        fclose(fp);
        return 0;
    }
    
    // read pictures
    for (int i = 0; i < data->num_pictures; i++) {
        Picture* pic = &data->pictures[i];
        if (fscanf(fp, "%d", &pic->id) != 1) {
            fprintf(stderr, "Error: could not read picture ID from file '%s'\n", filename);
            fclose(fp);
            return 0;
        }
        if (fscanf(fp, "%d", &pic->size) != 1) {
            fprintf(stderr, "Error: could not read picture size from file '%s'\n", filename);
            fclose(fp);
            return 0;
        }
        if (pic->size > MAX_PICTURE_SIZE) {
            fprintf(stderr, "Error: picture size is too large in file '%s'\n", filename);
            fclose(fp);
            return 0;
        }
        for (int y = 0; y < pic->size; y++) {
            for (int x = 0; x < pic->size; x++) {
                if (fscanf(fp, "%d", &pic->pixels[y][x]) != 1) {
                    fprintf(stderr, "Error: could not read pixel value from file '%s'\n", filename);
                    fclose(fp);
                    return 0;
                }
            }
        }
    }
}

int readObjects( FILE* fp,const char* filename, InputData* data)
{
// read number of objects
    if (fscanf(fp, "%d", &data->num_objects) != 1) {
        fprintf(stderr, "Error: could not read number of objects from file '%s'\n", filename);
        fclose(fp);
        return 0;
    }
    // allocate memory for objects
    data->objects = (Object*) malloc(data->num_objects * sizeof(Object));
    if (data->objects == NULL) {
        fprintf(stderr, "Error: could not allocate memory for objects\n");
        fclose(fp);
    
 	}
 	
 	// read objects
    for (int i = 0; i < data->num_objects; i++) {
        Object* obj = &data->objects[i];
        if (fscanf(fp, "%d", &obj->id) != 1) {
            fprintf(stderr, "Error: could not read object ID from file '%s'\n", filename);
            fclose(fp);
            return 0;
        }
        if (fscanf(fp, "%d", &obj->size) != 1) {
            fprintf(stderr, "Error: could not read object size from file '%s'\n", filename);
            fclose(fp);
            return 0;
        }
        if (obj->size > MAX_OBJECT_SIZE) {
            fprintf(stderr, "Error: object size is too large in file '%s'\n", filename);
            fclose(fp);
            return 0;
        }
        for (int y = 0; y < obj->size; y++) {
            for (int x = 0; x < obj->size; x++) {
                if (fscanf(fp, "%d", &obj->pixels[y][x]) != 1) {
                    fprintf(stderr, "Error: could not read pixel value from file '%s'\n", filename);
                    fclose(fp);
                    return 0;
                }
            }
        }
    }
}

void writeOutputFile(int** resultsMatrix, int rows, int cols) 
{
    FILE *fp;
    fp = fopen("Output.txt", "w");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }
    
    printf("Starting to write the output file\n");
    for (int i = 0; i < rows; i++) 
    {
        int numObjects = resultsMatrix[i][0];
     
        // write results to file
        fprintf(fp, "Picture %d: ", i+1);
        if (numObjects == 3) {
          fprintf(fp, "found Objects: %d Position(%d,%d) ; %d Position(%d,%d) ; %d Position(%d,%d)\n",
                    resultsMatrix[i][1], resultsMatrix[i][2], resultsMatrix[i][3],
                    resultsMatrix[i][4], resultsMatrix[i][5], resultsMatrix[i][6],
                    resultsMatrix[i][7], resultsMatrix[i][8], resultsMatrix[i][9]);
        } else {
            fprintf(fp, "No three different Objects were found\n");          
        }
    }
    printf("Done writing the output file\n");
    fclose(fp);
}

