#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "ppm.h"
#include "steganography.h"
#include "read_bin.h"

int main(int argc, char **argv) {
    // Seed PRNG used by some tests
    srand((unsigned) time(NULL));


if (strcmp(argv[1], "encode") ==0){
    if (argc <5){
        printf("\nERROR: Not enough arguments to encode.\n");
        return 1;
    }
    if ((strcmp(argv[2], argv[3]) == 0) || (strcmp(argv[2], argv[4]) ==0) || (strcmp(argv[3], argv[4]) ==0)){
        printf("\nERROR: Input, Payload and Output files all must have different names.\n");
        return 1;
    }

    encode_file(argv[2], argv[3], argv[4]);
}
if (strcmp(argv[1], "decode") ==0){
    if (argc < 4) {
        printf("\nERROR: Not enough arguments to decode.\n");
        return 1;
    }
    if (strcmp(argv[2], argv[3]) ==0){
        printf("\nERROR: Input and Output files must have different names.\n");
        return 1;
    }
    decode_file(argv[2], argv[3]);
}

    test_encode_bit();
    test_decode_bit();
    test_encode_byte();
    test_decode_byte();
    test_encode_decode();

    test_read_ppm();

    //encode_file("test_3x2.ppm", "payload.txt", "out_test.ppm");


    return 0;
    
}
