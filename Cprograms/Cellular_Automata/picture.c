#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "picture.h"
#include "arrays.h"
#include "rules.h"

void rule_18_picture(int width, int height){
    bool **array_2d = malloc_2d(width, height);
    initialize_first_row(array_2d[0], width);
    for (int i =1; i< height; ++i){
        //Left edge
        array_2d[i][0] = rule_18(false, array_2d[i - 1][0], array_2d[i - 1][1]);

        // middle
        for (int j = 1; j < width - 1; ++j) {
            bool left = array_2d[i-1][j-1];
            bool center = array_2d[i-1][j];
            bool right = array_2d[i-1][j+1];
            array_2d[i][j] = rule_18(left,center,right);
        }      
        // Right edge
        array_2d[i][width - 1] = rule_18(array_2d[i-1][width-2], array_2d[i-1][width-1],false);
    }
    printf("Rule 18 Picture\n");
    print_2d(array_2d,width, height);
    free_2d(array_2d, height);
}

void rule_57_picture(int width, int height){
   bool **array_2d = malloc_2d(width, height);
    initialize_first_row(array_2d[0], width);
    for (int i =1; i< height; ++i){
        //Left edge
        array_2d[i][0] = rule_57(false, array_2d[i - 1][0], array_2d[i - 1][1]);

        // middle
        for (int j = 1; j < width - 1; ++j) {
            bool left = array_2d[i-1][j-1];
            bool center = array_2d[i-1][j];
            bool right = array_2d[i-1][j+1];
            array_2d[i][j] = rule_57(left,center,right);
        }      
        // Right edge
        array_2d[i][width - 1] = rule_57(array_2d[i-1][width-2], array_2d[i-1][width-1],false);
    }
    printf("\n");
    printf("Rule 57 Picture\n");
    print_2d(array_2d,width, height);
    free_2d(array_2d, height);
}
void rule_60_picture(int width, int height){
    bool *array_row_major=malloc_row_major(width, height);
    initialize_first_row(array_row_major, width);
    for (int i =1; i< height; ++i){
        //Left edge
        array_row_major[row_major(i,0,width)] = rule_60(false, array_row_major[row_major(i - 1,0,width)], 
        array_row_major[row_major(i - 1, 1, width)]);

        // middle
        for (int j = 1; j < width - 1; ++j) {
            bool left = array_row_major[row_major(i-1, j-1, width)];
            bool center = array_row_major[row_major(i-1, j, width)];
            bool right = array_row_major[row_major(i-1, j+1, width)];
            array_row_major[row_major(i, j, width)] = rule_60(left,center,right);
        }      
        // Right edge
        array_row_major[row_major(i, width - 1, width)] = rule_60(array_row_major[row_major(i-1, width-2, width)], 
        array_row_major[row_major(i-1, width-1, width)],false);
    }
    printf("\n");
    printf("Rule 60 Picture\n");
    print_row_major(array_row_major, width, height);
    free_row_major(array_row_major);
    array_row_major = NULL;
}

void rule_73_picture(int width, int height){
    bool *array_row_major=malloc_row_major(width, height);
    initialize_first_row(array_row_major, width);
    for (int i =1; i< height; ++i){
        //Left edge
        array_row_major[row_major(i,0,width)] = rule_73(false, array_row_major[row_major(i - 1,0,width)], 
        array_row_major[row_major(i - 1, 1, width)]);

        // middle
        for (int j = 1; j < width - 1; ++j) {
            bool left = array_row_major[row_major(i-1, j-1, width)];
            bool center = array_row_major[row_major(i-1, j, width)];
            bool right = array_row_major[row_major(i-1, j+1, width)];
            array_row_major[row_major(i, j, width)] = rule_73(left,center,right);
        }      
        // Right edge
        array_row_major[row_major(i, width - 1, width)] = rule_73(array_row_major[row_major(i-1, width-2, width)], 
        array_row_major[row_major(i-1, width-1, width)],false);
    }
    printf("\n");
    printf("Rule 73 Picture\n");
    print_row_major(array_row_major, width, height);
    free_row_major(array_row_major);
    array_row_major = NULL;
}


void initialize_first_row(bool *array, int width){
    for (int i =0; i < width; ++i){
        array[i] = false;
    }
    int half = width/2;
    array[half] = true;
}
void print_2d(bool **array, int width, int height){
    for (int i=0; i< height; ++i){
        for (int j=0; j< width; ++j){
            if ((array[i][j]) == false){
                printf(".");
            }
            else{
                printf ("X");
            }
        }
        printf("\n");
    }
   
}
void print_row_major(bool *array, int width, int height){
    for (int i=0; i < height; ++i){
        for (int j =0; j< width; ++j){
            if ((array[row_major(i, j, width)]) == false){
                printf(".");
            }
            else{
                printf ("X");
            }
        }
        printf("\n");
    }
}

