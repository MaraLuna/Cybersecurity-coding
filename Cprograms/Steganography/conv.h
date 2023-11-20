#ifndef CONV_H
#define CONV_H

#include <stdbool.h>
#include <stdint.h>

bool safe_string_to_int(char *input, int *output);
bool safe_string_to_uint8(char *input, uint8_t *output);

#endif
