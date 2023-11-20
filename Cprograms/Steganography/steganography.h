#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include <stdint.h>
#include "ppm.h"

void encode_file(char *in_ppm, char *payload, char *out_ppm);
void decode_file(char *in_ppm, char *message);

void encode_bit(PPM_Pixel *p, uint8_t bit);
void test_encode_bit(void);
void test_encode_bit1(void);
void test_encode_bit2(void);
void test_encode_bit3(void);

void encode_byte(PPM_Pixel *p, uint8_t byte);
void test_encode_byte(void);
void test_encode_byte1(void);
void test_encode_byte2(void);
void test_encode_byte3(void);

uint8_t decode_bit(PPM_Pixel p);
void test_decode_bit1(void);
void test_decode_bit2(void);
void test_decode_bit(void);

uint8_t decode_byte(PPM_Pixel *p);
void test_decode_byte(void);
void test_decode_byte1(void);

void test_encode_decode(void);
void test_encode_decode_random(void);

#endif
