#include "ppm.h"

#include <inttypes.h>
#include <stdio.h>

#include "conv.h"

bool read_ppm_header(FILE *input, PPM_Header *header) {
    char width_s[1000];
    char height_s[1000];
    char max_pixel_s[300];
    int count = fscanf(input, "P3 %49s %49s %49s ", width_s, height_s, max_pixel_s);
   
    if (count != 3) {
        fprintf(stderr, "Unable to read header\n");
        return false;
    }
    if (!safe_string_to_int(width_s, &header->width)) {
        fprintf(stderr, "Unable to convert width\n");
        return false;
    }
    if (!safe_string_to_int(height_s, &header->height)) {
        fprintf(stderr, "Unable to convert height\n");
        return false;
    }
    if (!safe_string_to_int(max_pixel_s, &header->max_pixels)) {
        fprintf(stderr, "Unable to convert pixels\n");
        return false;
    }
    //printf("max pixels is %d\n", header->max_pixels);
    if (header ->max_pixels > 255){
        fprintf(stderr,"Maximum value for color pixels is 255. Format unsupported.\n");
        return false;
    }

    return true;
}

bool write_ppm_header(FILE *output, PPM_Header *header) {
    if (fprintf(output, "P3 %d %d %d\n", header->width, header->height, header ->max_pixels) < 0) {
        return false;
    }
    return true;
}

bool read_ppm_pixel(FILE *input, PPM_Pixel *pixel) {
    char red[5];
    char green[5];
    char blue[5];

    int count = fscanf(input, "%4s %4s %4s", red, green, blue);

    if (count != 3) {
        fprintf(stderr, "Unable to read pixel\n");
        return false;
    }
    if (!safe_string_to_uint8(red, &pixel->red)) {
        fprintf(stderr, "Unable to convert red\n");
        return false;
    }
    if (!safe_string_to_uint8(green, &pixel->green)) {
        fprintf(stderr, "Unable to convert green\n");
        return false;
    }
    if (!safe_string_to_uint8(blue, &pixel->blue)) {
        fprintf(stderr, "Unable to convert blue\n");
        return false;
    }

    return true;
}

bool write_ppm_pixel(FILE *output, PPM_Pixel *pixel) {
    if (fprintf(output, "%" PRIu8 " %" PRIu8 " %" PRIu8 "\n",
                pixel->red, pixel->green, pixel->blue) < 0) {
        return false;
    }
    return true;
}

bool read_8_pixels(FILE *input, PPM_Pixel *pixels){
    for (int i = 0; i < 8; i++){
      if (!read_ppm_pixel(input, &pixels[i])) {
        return false;
      }
    }
    return true;
}

bool write_8_pixels(FILE *output, PPM_Pixel *pixels){
    for (int i = 0; i < 8; i++){
        if (!write_ppm_pixel(output, &pixels[i])){
        return false;
        }   
    }
    return true;
}

void test_read_ppm(void) {
    // Open file
    FILE *input = fopen("test_3x2.ppm", "r");
    if (input == NULL) {
        perror("fopen() failure");
        return;
    }

    // Read and verify header
    PPM_Header header;
    read_ppm_header(input, &header);

    if (header.width != 3) {
        fprintf(stderr, "Incorrect width read, expected 3, found %d\n",
                header.width);
        fclose(input);
        return;
    }
    if (header.height != 2) {
        fprintf(stderr, "Incorrect height read, expected 2, found %d\n",
                header.height);
        fclose(input);
        return;
    }

    // Read and verify pixels
    PPM_Pixel pixel;
    read_ppm_pixel(input, &pixel);
    if (pixel.red != 255 || pixel.green != 0 || pixel.blue != 0) {
        fprintf(stderr, "Error pixel 0, read (%" PRIu8 ", %" PRIu8
                ", %" PRIu8 "), expected (255, 0, 0)\n",
                pixel.red, pixel.green, pixel.blue);
    }
    read_ppm_pixel(input, &pixel);
    if (pixel.red != 0 || pixel.green != 255 || pixel.blue != 0) {
        fprintf(stderr, "Error pixel 1, read (%" PRIu8 ", %" PRIu8
                ", %" PRIu8 "), expected (0, 255, 0)\n",
                pixel.red, pixel.green, pixel.blue);
    }
    read_ppm_pixel(input, &pixel);
    if (pixel.red != 0 || pixel.green != 0 || pixel.blue != 255) {
        fprintf(stderr, "Error pixel 2, read (%" PRIu8 ", %" PRIu8
                ", %" PRIu8 "), expected (0, 0, 255)\n",
                pixel.red, pixel.green, pixel.blue);
    }

    // Close file
    fclose(input);
}

