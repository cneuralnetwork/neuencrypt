#include "polyalphabetic.h"

int vigenere_encrypt(const char* plaintext, const char* key, char* ciphertext) {
    if (!plaintext || !key || !ciphertext) return ERROR_INVALID_INPUT;
    if (strlen(key) == 0) return ERROR_INVALID_KEY;
    
    int key_len = strlen(key);
    int i = 0, j = 0;
    
    while (plaintext[i]) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            int shift = toupper(key[j % key_len]) - 'A';
            ciphertext[i] = ((toupper(plaintext[i]) - 'A' + shift) % 26) + base;
            j++;
        } else {
            ciphertext[i] = plaintext[i];
        }
        i++;
    }
    ciphertext[i] = '\0';
    
    return SUCCESS;
}

int vigenere_decrypt(const char* ciphertext, const char* key, char* plaintext) {
    if (!ciphertext || !key || !plaintext) return ERROR_INVALID_INPUT;
    if (strlen(key) == 0) return ERROR_INVALID_KEY;
    
    int key_len = strlen(key);
    int i = 0, j = 0;
    
    while (ciphertext[i]) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            int shift = toupper(key[j % key_len]) - 'A';
            plaintext[i] = ((toupper(ciphertext[i]) - 'A' - shift + 26) % 26) + base;
            j++;
        } else {
            plaintext[i] = ciphertext[i];
        }
        i++;
    }
    plaintext[i] = '\0';
    
    return SUCCESS;
}

void polyalphabetic_cipher_menu(void) {
    char* input = get_user_input("Enter text: ", MAX_INPUT_LENGTH);
    if (!input) {
        printf("Error: Failed to get input\n");
        return;
    }

    char* key = get_user_input("Enter key (letters only): ", MAX_KEY_LENGTH);
    if (!key) {
        printf("Error: Failed to get key\n");
        free(input);
        return;
    }

    str_to_upper(key);

    char* result = malloc(strlen(input) + 1);
    if (!result) {
        printf("Error: Memory allocation failed\n");
        free(input);
        free(key);
        return;
    }

    printf("\nSelect operation:\n");
    printf("1. Encrypt\n");
    printf("2. Decrypt\n");
    printf("Choice: ");

    int choice;
    scanf("%d", &choice);
    clear_input_buffer();

    int status;
    if (choice == 1) {
        status = vigenere_encrypt(input, key, result);
        printf("\nEncrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else if (choice == 2) {
        status = vigenere_decrypt(input, key, result);
        printf("\nDecrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else {
        printf("Invalid choice\n");
    }

    free(input);
    free(key);
    free(result);
}
