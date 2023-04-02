#include "header.h"
// This function gets a picture and the array of objects and searches for 3 objects in that picture
int SearchObjectsInPicture(int* resultRow,Picture pic, Object* objects,int numObjects, float matching_value)
{
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
							printf("\t\tObject %d found in picture %d indices (%d,%d)\n", objects[k].id, pic.id, i,j);
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


int checkObjectInPosition(Picture pic,Object obj,int row,int column, float matching_value)
{
	/*
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
	*/
	
	return 0;	

}
