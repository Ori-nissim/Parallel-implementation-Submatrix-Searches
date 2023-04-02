// Header
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_PICTURE_SIZE 500
#define MAX_OBJECT_SIZE 300
#define RESULT_COLUMN 50

#define CHECK_MALLOC(ptr) if (!ptr) { \
                              fprintf(stderr, "Memory allocation failed\n"); \
                              exit(EXIT_FAILURE); \
                          }

typedef struct {
    int id;
    int size;
    int pixels[MAX_PICTURE_SIZE][MAX_PICTURE_SIZE];
} Picture;

typedef struct {
    int id;
    int size;
    int pixels[MAX_OBJECT_SIZE][MAX_OBJECT_SIZE];
} Object;

typedef struct InputData{
    float matching_value;
    int num_pictures;
    Picture* pictures;
    int num_objects;
    Object* objects;
} InputData;

int readInputFile(const char* filename, InputData* data);
int readPictures( FILE* fp,const char* filename, InputData* data);
int readObjects( FILE* fp,const char* filename, InputData* data);
int SearchObjectsInPicture(int* resultRow,Picture pic, Object* objects,int numObjects, float matching_value);
int checkObjectInPosition(Picture pic,Object obj,int row,int column, float matching_value);
void writeOutputFile(int** resultsMatrix, int rows, int cols);

//utility
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

