#include "header.h"
void freeResultMatrix(int** resultMatrix, int numPics) {
    for (int i = 0; i < numPics; i++) {
        free(resultMatrix[i]);
    }
    free(resultMatrix);
}

void printArray(int** arr, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}
void freeInputData(InputData* inputData) {
    for (int i = 0; i < inputData->num_pictures; i++) {
        free(inputData->pictures[i].pixels);
    }
    free(inputData->pictures);
    for (int i = 0; i < inputData->num_objects; i++) {
        free(inputData->objects[i].pixels);
    }
    free(inputData->objects);
    free(inputData);
}

void freeData(Object* objects, Picture* allPictures, int numPics) {
    // Free memory allocated for objects
    free(objects);

    // Free memory allocated for allPictures
    for (int i = 0; i < numPics; i++) {
        free(allPictures[i].pixels);
    }
    free(allPictures);
}
