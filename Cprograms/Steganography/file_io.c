#include "file_io.h"

// Std library headers
#include <errno.h>
#include <stdio.h>

// Library headers
#include <sys/stat.h> // POSIX header

// Your own headers

uint16_t file_size(char *filename) {
    struct stat info;
    errno = 0; // Reset errno to be careful
    if (stat(filename, &info) != 0) {
        perror("stat() failed");
        return 0;
    }
    return (uint16_t) info.st_size;
}
