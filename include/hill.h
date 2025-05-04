#ifndef HILL_H
#define HILL_H

#include "utils.h"

#define MATRIX_SIZE 2  

typedef struct {
    int matrix[MATRIX_SIZE][MATRIX_SIZE];
    int det; 
} HillKey;

int hill_encrypt(const char* plaintext, const HillKey* key, char* ciphertext);
int hill_decrypt(const char* ciphertext, const HillKey* key, char* plaintext);
void hill_cipher_menu(void);
HillKey* generate_hill_key(void);
int mod_inverse(int a, int m);

#endif 