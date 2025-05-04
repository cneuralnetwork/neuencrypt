#ifndef HOMOPHONIC_H
#define HOMOPHONIC_H

#include "utils.h"

#define NUM_SUBSTITUTIONS 4 

typedef struct {
    char letter;
    char substitutions[NUM_SUBSTITUTIONS];
} HomophonicMapping;

int homophonic_encrypt(const char* plaintext, const HomophonicMapping* key, char* ciphertext);
int homophonic_decrypt(const char* ciphertext, const HomophonicMapping* key, char* plaintext);
void homophonic_cipher_menu(void);
HomophonicMapping* generate_homophonic_key(void);

#endif