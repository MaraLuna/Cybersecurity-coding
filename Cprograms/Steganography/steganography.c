#include "steganography.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "ppm.h"

void encode_file(char *in_ppm_name, char *payload_name, char *out_ppm_name) {
  // Open All files
  FILE *in_ppm = NULL;
  FILE *out_ppm = NULL;
  FILE *payload = NULL;
  in_ppm = fopen(in_ppm_name, "r");
  if (in_ppm == NULL) {
    perror("Unable to fopen() in_ppm");
    goto encode_error;
  }
  out_ppm = fopen(out_ppm_name, "w");
  if (out_ppm == NULL) {
    perror("Unable to fopen() out_ppm");
    goto encode_error;
  }
  payload = fopen(payload_name, "rb");
  if (payload == NULL) {
    perror("Unable to fopen() payload");
    goto encode_error;
  }

  // Read / write header
  PPM_Header header;
  if (!read_ppm_header(in_ppm, &header)) {
    fprintf(stderr, "Error reading header\n");
    goto encode_error;
  }
  if (!write_ppm_header(out_ppm, &header)) {
    fprintf(stderr, "Error writing header\n");
    goto encode_error;
  }

  // Setup for processing pixels
  int num_pixels = header.width * header.height;
  int current_in_pixel = 0;

  // Find Payload size
  struct stat st;
  stat(payload_name, &st);
  uint16_t size_message = st.st_size;
  printf("File size: %d bytes\n", size_message);
  int size_to_encode = ((2+size_message)*8);

  if (size_to_encode > num_pixels){
    printf("Payloads exceeds available space in input file.\n");
    exit(0);
  }

  //Convert Payload Size to UINT8 and write out
  uint8_t payload_convA = (uint8_t)((size_message & 0xFF00) >> 8);
  uint8_t payload_convB= (uint8_t)(size_message & 0x00FF);
  printf("Payload_conv_A = %d\n", payload_convA);
  printf("Payload_conv_B = %d\n", payload_convB);

PPM_Pixel pixels[8];
read_8_pixels(in_ppm, pixels);
encode_byte(pixels, payload_convA);
write_8_pixels(out_ppm, pixels);
read_8_pixels(in_ppm, pixels);
encode_byte(pixels, payload_convB);
write_8_pixels(out_ppm, pixels);
current_in_pixel += 16;

  // Read file byte by byte

  for (;;) {
    uint8_t payload_byte;
    if (fread(&payload_byte, sizeof(uint8_t), 1, payload) < 1) {
        // hit EOF
        break;
    }
    PPM_Pixel pixels[8];
    read_8_pixels(in_ppm, pixels);
    encode_byte(pixels, payload_byte);
    write_8_pixels(out_ppm, pixels);
    current_in_pixel = current_in_pixel+8;
      
    //printf("Read byte 0x%x\n",(unsigned) payload_byte);
  }

 // Write remaining pixels of in_ppm to out_ppm
  // i may start at other than 0 based upon the size of the payload
  for (int i = current_in_pixel; i < num_pixels; ++i) {
    PPM_Pixel pixel;
    // Read a pixel
    if (!read_ppm_pixel(in_ppm, &pixel)) {
      fprintf(stderr, "Error reading pixel %d\n", i);
      goto encode_error;
    }
    // Write a pixel
    if (!write_ppm_pixel(out_ppm, &pixel)) {
      fprintf(stderr, "Error writing pixel %d\n", i);
      goto encode_error;
    }
  }
  // Close everything
  fclose(in_ppm);
  fclose(out_ppm);
  fclose(payload);

  return;

// What should happen on an error
encode_error:
  if (in_ppm != NULL) {
    fclose(in_ppm);
  }
  if (out_ppm != NULL) {
    fclose(out_ppm);
  }
  if (payload != NULL) {
    fclose(payload);
  }
  fprintf(stderr, "Fatal error in encode()\n");
  exit(1);
}

void encode_bit(PPM_Pixel *p, uint8_t bit) {
  // Check if LSB of green xor LSB of blue matches bit
  // If not, toggle LSB of blue
  if (bit != ((p->green & 0x1) ^ (p->blue & 0x1))){
    (p->blue ^= 0x1);
  }

}

void encode_byte(PPM_Pixel *p, uint8_t byte) {
  for (int i = 7; i >= 0; --i) {
    uint8_t bit = (byte >> i) & 0x1;
    encode_bit(&p[i], bit);
  }
}
//********************************************************
//********************************************************
//********************    DECODING    ********************
//********************************************************
//********************************************************

void decode_file(char *in_secret_ppm, char *hidden_message_name){
 // Open All files
  FILE *in_ppm = NULL;
  FILE *message = NULL;
  in_ppm = fopen(in_secret_ppm, "r");
  if (in_ppm == NULL) {
    perror("Unable to fopen() in_ppm");
    goto encode_error;
  }
  message = fopen(hidden_message_name, "wb");
  if (message == NULL) {
    perror("Unable to fopen() message");
    goto encode_error;
  }
  // Read header
  PPM_Header header;
  if (!read_ppm_header(in_ppm, &header)) {
    fprintf(stderr, "Error reading header\n");
    goto encode_error;
  }

//Read 8 pixels and decode to get top half of size
PPM_Pixel pixels[8];
read_8_pixels(in_ppm, pixels);
uint8_t partA = decode_byte(pixels);

//Read 8 pixels and decode to get bottom half of size
read_8_pixels(in_ppm, pixels);
uint8_t partB = decode_byte(pixels);

printf("PartA = %d\n", partA);
printf("PartB = %d\n", partB);
//Reconstruct size from two halfs
uint16_t message_length = (partA << 8) | partB;
printf("Message_length = %d\n", message_length);

//For each byte specified by the size, read 8 pixels, decode a byte and write to output
for (int i=0; i < message_length; ++i){
  read_8_pixels(in_ppm, pixels);
  uint8_t byte = decode_byte(pixels);
  fwrite(&byte, sizeof(uint8_t), 1, message);
}

// Close everything
  fclose(in_ppm);
  fclose(message);


  return;

// What should happen on an error
encode_error:
  if (in_ppm != NULL) {
    fclose(in_ppm);
  }
  if (message != NULL) {
    fclose(message);
  }
  fprintf(stderr, "Fatal error in decode()\n");
  exit(1);


}

uint8_t decode_bit(PPM_Pixel p) { return (p.green & 0x1) ^ (p.blue & 0x1); }

uint8_t decode_byte(PPM_Pixel *p) {
  uint8_t byte = 0;

  for (int i = 7; i >= 0; --i) {
    uint8_t bit = decode_bit(p[i]);
    byte |= bit << i;
  }

  return byte;
}

// All tests below
void test_encode_bit1(void) {
  PPM_Pixel p;
  p.red = 0;
  p.green = 0;
  p.blue = 0;
  uint8_t bit = 0;
  encode_bit(&p, bit);
  if (p.red != 0 || p.green != 0 || p.blue != 0) {
    printf("Error test_encode_bit 1\n");
  }
}
void test_encode_bit2(void) {
  PPM_Pixel p;
  p.red = 0;
  p.green = 0;
  p.blue = 0;
  uint8_t bit = 1;
  encode_bit(&p, bit);
  if (p.red != 0 || p.green != 0 || p.blue != 1) {
    printf("Error test_encode_bit 2\n");
  }
}
void test_encode_bit3(void) {
  PPM_Pixel p;
  p.red = 0;
  p.green = 0;
  p.blue = 1;
  uint8_t bit = 1;
  encode_bit(&p, bit);
  if (p.red != 0 || p.green != 0 || p.blue != 1) {
    printf("Error test_encode_bit 3\n");
  }
}

void test_encode_bit(void) {
  test_encode_bit1();
  test_encode_bit2();
  test_encode_bit3();
}
void test_encode_byte1(void) {
  int test_num = 1;
  PPM_Pixel p[8];
  p[0].red = 0;
  p[0].green = 0;
  p[0].blue = 0;
  p[1].red = 0;
  p[1].green = 0;
  p[1].blue = 0;
  p[2].red = 0;
  p[2].green = 0;
  p[2].blue = 0;
  p[3].red = 0;
  p[3].green = 0;
  p[3].blue = 0;
  p[4].red = 0;
  p[4].green = 0;
  p[4].blue = 0;
  p[5].red = 0;
  p[5].green = 0;
  p[5].blue = 0;
  p[6].red = 0;
  p[6].green = 0;
  p[6].blue = 0;
  p[7].red = 0;
  p[7].green = 0;
  p[7].blue = 0;

  PPM_Pixel p_copy[8];
  for (int i = 0; i < 8; ++i) {
    p_copy[i] = p[i];
  }

  uint8_t byte = 0xFF;

  encode_byte(p_copy, byte);

  // Ensure red / green not modified
  for (int i = 0; i < 8; ++i) {
    if (p[i].red != p_copy[i].red) {
      printf("Error test_encode_byte %i, pixel %i has differing red values\n",
             test_num, i);
    }
    if (p[i].green != p_copy[i].green) {
      printf("Error test_encode_byte %i, pixel %i has differing green values\n",
             test_num, i);
    }
  }

  // Ensure blue matches what it should
  if (p_copy[0].blue != 1 || p_copy[1].blue != 1 || p_copy[2].blue != 1 ||
      p_copy[3].blue != 1 || p_copy[4].blue != 1 || p_copy[5].blue != 1 ||
      p_copy[6].blue != 1 || p_copy[7].blue != 1) {
    printf("Error test_encode_byte %i, blue values not expected\n", test_num);
  }
}
void test_encode_byte2(void) {
  int test_num = 2;
  PPM_Pixel p[8];
  p[0].red = 255;
  p[0].green = 255;
  p[0].blue = 255;
  p[1].red = 255;
  p[1].green = 255;
  p[1].blue = 255;
  p[2].red = 255;
  p[2].green = 255;
  p[2].blue = 255;
  p[3].red = 255;
  p[3].green = 255;
  p[3].blue = 255;
  p[4].red = 255;
  p[4].green = 255;
  p[4].blue = 255;
  p[5].red = 255;
  p[5].green = 255;
  p[5].blue = 255;
  p[6].red = 255;
  p[6].green = 255;
  p[6].blue = 255;
  p[7].red = 255;
  p[7].green = 255;
  p[7].blue = 255;

  PPM_Pixel p_copy[8];
  for (int i = 0; i < 8; ++i) {
    p_copy[i] = p[i];
  }

  uint8_t byte = 0xFF;

  encode_byte(p_copy, byte);

  // Ensure red / green not modified
  for (int i = 0; i < 8; ++i) {
    if (p[i].red != p_copy[i].red) {
      printf("Error test_encode_byte %i, pixel %i has differing red values\n",
             test_num, i);
    }
    if (p[i].green != p_copy[i].green) {
      printf("Error test_encode_byte %i, pixel %i has differing green values\n",
             test_num, i);
    }
  }

  // Ensure blue matches what it should
  if (p_copy[0].blue != 254 || p_copy[1].blue != 254 || p_copy[2].blue != 254 ||
      p_copy[3].blue != 254 || p_copy[4].blue != 254 || p_copy[5].blue != 254 ||
      p_copy[6].blue != 254 || p_copy[7].blue != 254) {
    printf("Error test_encode_byte %i, blue values not expected\n", test_num);
  }
}
void test_encode_byte3(void) {
  int test_num = 3;
  PPM_Pixel p[8];
  p[0].red = 255;
  p[0].green = 255;
  p[0].blue = 255;
  p[1].red = 255;
  p[1].green = 255;
  p[1].blue = 255;
  p[2].red = 255;
  p[2].green = 255;
  p[2].blue = 255;
  p[3].red = 255;
  p[3].green = 255;
  p[3].blue = 255;
  p[4].red = 255;
  p[4].green = 255;
  p[4].blue = 255;
  p[5].red = 255;
  p[5].green = 255;
  p[5].blue = 255;
  p[6].red = 255;
  p[6].green = 255;
  p[6].blue = 255;
  p[7].red = 255;
  p[7].green = 255;
  p[7].blue = 255;

  PPM_Pixel p_copy[8];
  for (int i = 0; i < 8; ++i) {
    p_copy[i] = p[i];
  }

  uint8_t byte = 0xAA;

  encode_byte(p_copy, byte);

  // Ensure red / green not modified
  for (int i = 0; i < 8; ++i) {
    if (p[i].red != p_copy[i].red) {
      printf("Error test_encode_byte %i, pixel %i has differing red values\n",
             test_num, i);
    }
    if (p[i].green != p_copy[i].green) {
      printf("Error test_encode_byte %i, pixel %i has differing green values\n",
             test_num, i);
    }
  }

  // Ensure blue matches what it should
  if (p_copy[0].blue != 255 || p_copy[1].blue != 254 || p_copy[2].blue != 255 ||
      p_copy[3].blue != 254 || p_copy[4].blue != 255 || p_copy[5].blue != 254 ||
      p_copy[6].blue != 255 || p_copy[7].blue != 254) {
    printf("Error test_encode_byte %i, blue values not expected\n", test_num);
  }
}
void test_encode_byte(void) {
  test_encode_byte1();
  test_encode_byte2();
  test_encode_byte3();
}

void test_decode_bit1(void) {
  PPM_Pixel p;
  p.red = 0;
  p.green = 0;
  p.blue = 0;
  if (decode_bit(p) != 0) {
    printf("Error test_decode_bit 1\n");
  }
}
void test_decode_bit2(void) {
  PPM_Pixel p;
  p.red = 0;
  p.green = 0;
  p.blue = 1;
  if (decode_bit(p) != 1) {
    printf("Error test_decode_bit 2\n");
  }
}

void test_decode_bit(void) {
  test_decode_bit1();
  test_decode_bit2();
}

void test_decode_byte1(void) {
  int test_num = 1;
  PPM_Pixel p[8];
  p[0].red = 0;
  p[0].green = 0;
  p[0].blue = 0;
  p[1].red = 0;
  p[1].green = 0;
  p[1].blue = 0;
  p[2].red = 0;
  p[2].green = 0;
  p[2].blue = 0;
  p[3].red = 0;
  p[3].green = 0;
  p[3].blue = 0;
  p[4].red = 0;
  p[4].green = 0;
  p[4].blue = 0;
  p[5].red = 0;
  p[5].green = 0;
  p[5].blue = 0;
  p[6].red = 0;
  p[6].green = 0;
  p[6].blue = 0;
  p[7].red = 0;
  p[7].green = 0;
  p[7].blue = 0;

  uint8_t byte = decode_byte(p);
  uint8_t expected = 0x00;

  if (byte != expected) {
    printf("Error test_decode_byte %d\n", test_num);
  }
}

void test_decode_byte(void) { test_decode_byte1(); }

void test_encode_decode(void) {
  for (int i = 0; i < 100; ++i) {
    test_encode_decode_random();
  }
}
void test_encode_decode_random(void) {
  // Generate an array of random pixels
  // Here the mod doesn't introduce modulo bias in practice because
  // rand() generates in a range that is a power of 2 and modding by
  // a power of 2 (UINT8_MAX) will always work since there is no
  // remainder when dividing RAND_MAX / UINT8_MAX
  PPM_Pixel pixels[8];
  for (int i = 0; i < 8; ++i) {
    pixels[i].red = (uint8_t)rand() % UINT8_MAX;
    pixels[i].green = (uint8_t)rand() % UINT8_MAX;
    pixels[i].blue = (uint8_t)rand() % UINT8_MAX;
  }

  // Generate a random byte
  uint8_t byte = (uint8_t)rand() % UINT8_MAX;

  // Encode
  encode_byte(pixels, byte);

  // Decode
  uint8_t decoded = decode_byte(pixels);

  // Verify
  if (byte != decoded) {
    printf("Error in encode_decode_random\n");
    printf("Started with %x\n", byte);
    printf("Decoded %x\n", decoded);
  }
}
