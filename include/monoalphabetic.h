#ifndef MONOALPHABETIC_H
#define MONOALPHABETIC_H

#include "utils.h"

int monoalphabetic_encrypt(const char* plaintext, const char* key, char* ciphertext);
int monoalphabetic_decrypt(const char* ciphertext, const char* key, char* plaintext);
void monoalphabetic_cipher_menu(void);
char* generate_random_key(void);

#endif 