#include "conv.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Relevant SEI CERT Secure C Coding rule
// https://wiki.sei.cmu.edu/confluence/display/c/ERR34-C.+Detect+errors+when+converting+a+string+to+a+number

bool safe_string_to_int(char *input, int *output) {
    // Clear out in case it was previously set
    errno = 0;

    // Perform conversion
    char *end;
    const long sl = strtol(input, &end, 10);

    // Validation
    if (end == input) {
        fprintf(stderr, "%s: not a decimal number\n", input);
        return false;
    } else if ('\0' != *end) {
        fprintf(stderr, "%s: extra characters at end of input: %s\n", input, end);
        return false;
    } else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno) {
        fprintf(stderr, "%s out of range of type long\n", input);
        return false;
    } else if (sl > INT_MAX) {
        fprintf(stderr, "%ld greater than INT_MAX\n", sl);
        return false;
    } else if (sl < INT_MIN) {
        fprintf(stderr, "%ld less than INT_MIN\n", sl);
        return false;
    }
    
    // Have a valid int
    *output = (int) sl;
    return true;
}

bool safe_string_to_uint8(char *input, uint8_t *output) {
    // Clear out in case it was previously set
    errno = 0;

    // Perform conversion
    char *end;
    const long sl = strtol(input, &end, 10);

    // Validation
    if (end == input) {
        fprintf(stderr, "%s: not a decimal number\n", input);
        return false;
    } else if ('\0' != *end) {
        fprintf(stderr, "%s: extra characters at end of input: %s\n", input, end);
        return false;
    } else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno) {
        fprintf(stderr, "%s out of range of type long\n", input);
        return false;
    } else if (sl > UINT8_MAX) {
        fprintf(stderr, "%ld greater than UINT8_MAX\n", sl);
        return false;
    } else if (sl < 0) {
        fprintf(stderr, "%ld less than 0\n", sl);
        return false;
    }
    
    // Have a valid int
    *output = (uint8_t) sl;
    return true;
}



