#include "read_bin.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

void read_file_in_bin(char *filename) {
    FILE *in = fopen(filename, "rb");
    if (in == NULL) {
        perror("Unable to open file");
        return;
    }

    // Read file byte by byte
    for (;;) {
        uint8_t byte;
        if (fread(&byte, sizeof(uint8_t), 1, in) < 1) {
            // hit EOF
            break;
        }

    //printf("Read byte 0x%x\n",(unsigned) byte);
    }

    fclose(in);
}
