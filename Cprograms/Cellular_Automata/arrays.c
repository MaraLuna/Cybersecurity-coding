#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "arrays.h"

bool **malloc_2d(int width, int height){
    bool **array = malloc((unsigned) height * sizeof(int *));
    for (int i=0; i < height; ++i){
        array[i] = malloc((unsigned)width * sizeof(int));
    }
    return array;
}

void free_2d(bool **array, int height){
    for (int i=0; i < height; ++i){
        free(array[i]);
        array[i] = NULL;
    }
    free(array);
    array = NULL;
}

bool *malloc_row_major(int width, int height){
    return malloc((unsigned)(width * height) * sizeof(bool));
}
void free_row_major(bool *array){
    free(array);
    array = NULL;
}
int row_major(int row, int col, int width){
    return row * width + col;
}