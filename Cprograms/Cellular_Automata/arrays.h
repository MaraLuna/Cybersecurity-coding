#ifndef ARRAYS_H
#define ARRAYS_H

bool **malloc_2d(int width, int height);
void free_2d(bool **array, int height);
bool *malloc_row_major(int width, int height);
void free_row_major(bool *array);
int row_major(int row, int col, int width);

#endif
