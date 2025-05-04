#ifndef ENCODINGS_H
#define ENCODINGS_H

#include "utils.h"
#include <openssl/evp.h>
#include <openssl/err.h>

int base64_encode(const unsigned char* input, size_t length, char* output);
int base64_decode(const char* input, unsigned char* output, size_t* output_length);

int base32_encode(const unsigned char* input, size_t length, char* output);
int base32_decode(const char* input, unsigned char* output, size_t* output_length);

int hex_encode(const unsigned char* input, size_t length, char* output);
int hex_decode(const char* input, unsigned char* output, size_t* output_length);

int calculate_hash(const unsigned char* input, size_t length, unsigned char* output, 
                  size_t* output_length, const char* algorithm);

void encoding_menu(void);

#endif 