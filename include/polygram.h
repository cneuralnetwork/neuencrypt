#ifndef POLYGRAM_H
#define POLYGRAM_H

#include "utils.h"
#include <time.h>

#define DIGRAPH_SIZE 2

typedef struct {
    char digraph[DIGRAPH_SIZE + 1];
    char substitution[DIGRAPH_SIZE + 1];
} DigraphMapping;

int polygram_encrypt(const char* plaintext, const DigraphMapping* key, int key_size, char* ciphertext);
int polygram_decrypt(const char* ciphertext, const DigraphMapping* key, int key_size, char* plaintext);
void polygram_cipher_menu(void);
DigraphMapping* generate_digraph_key(int* key_size);

#endif 