#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#include "utils.h"

#define GRID_SIZE 5

typedef struct {
    char grid[GRID_SIZE][GRID_SIZE];
} PlayfairKey;

int playfair_encrypt(const char* plaintext, const PlayfairKey* key, char* ciphertext);
int playfair_decrypt(const char* ciphertext, const PlayfairKey* key, char* plaintext);
void playfair_cipher_menu(void);
PlayfairKey* generate_playfair_key(const char* keyword);

#endif 