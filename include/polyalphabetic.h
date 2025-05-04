#ifndef POLYALPHABETIC_H
#define POLYALPHABETIC_H

#include "utils.h"

int vigenere_encrypt(const char* plaintext, const char* key, char* ciphertext);
int vigenere_decrypt(const char* ciphertext, const char* key, char* plaintext);
void polyalphabetic_cipher_menu(void);

#endif 