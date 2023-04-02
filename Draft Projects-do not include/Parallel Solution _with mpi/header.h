#pragma once
// Header

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define SUCCESS 1
#define FAILURE 0

// ----- CUDA ----

#define WARP_SIZE 32
#define NUM_THREADS 3840

// ---------------
#define MAX_PICTURE_SIZE 500
#define MAX_OBJECT_SIZE 300					// int            int 		int	int		int   // therefor we will need 3 int*3 objects + 1 object counter = 10 ints
#define RESULT_COLUMN 10 // structure example for row 0(pic1) : [num of objects] [obj id][position x] [position y] [obj id] ...

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
int SearchObjectsInPicture(int* resultRow,Picture pic, Object* objects,int numObjects, float matching_value,int rank);
int checkObjectInPosition(Picture pic,Object obj,int row,int column, float matching_value);

// mpi
void MPIsendData(int numObjs, Object* objects, int destinationRank,float matching_value, int numPics);
Object* MPIrecvData(int sourceRank,int* numObjs, float* matching_value, int* numPics);
void sendJob(Picture picture, int tag);
void recvJob(Picture* picture, int* tag);

void printPicture(Picture pic);
void sortPicturesBySize(Picture pictures[], int numPictures);

