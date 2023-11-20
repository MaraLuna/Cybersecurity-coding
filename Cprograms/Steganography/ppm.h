#ifndef PPM_H
#define PPM_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    int width;
    int height;
    int max_pixels;
} PPM_Header;

bool read_ppm_header(FILE *input, PPM_Header *header);
bool write_ppm_header(FILE *output, PPM_Header *header);

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} PPM_Pixel;


bool read_ppm_pixel(FILE *input, PPM_Pixel *pixel);
bool write_ppm_pixel(FILE *output, PPM_Pixel *pixel);
bool read_8_pixels(FILE *input, PPM_Pixel *pixels);
bool write_8_pixels(FILE *output, PPM_Pixel *pixels);

void test_read_ppm(void);

#endif
