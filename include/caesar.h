#ifndef CAESAR_H
#define CAESAR_H

#include "utils.h"


int caesar_encrypt(const char* plaintext, int shift, char* ciphertext);
int caesar_decrypt(const char* ciphertext, int shift, char* plaintext);
void caesar_cipher_menu(void);

#endif 