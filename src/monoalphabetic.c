#include "monoalphabetic.h"
#include <time.h>

char* generate_random_key(void) {
    char* key = malloc(27);
    if (!key) return NULL;
    
    for (int i = 0; i < 26; i++) {
        key[i] = 'A' + i;
    }
    key[26] = '\0';
    
    srand(time(NULL));
    for (int i = 25; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = key[i];
        key[i] = key[j];
        key[j] = temp;
    }
    
    return key;
}

int monoalphabetic_encrypt(const char* plaintext, const char* key, char* ciphertext) {
    if (!plaintext || !key || !ciphertext) return ERROR_INVALID_INPUT;
    if (strlen(key) != 26) return ERROR_INVALID_KEY;
    
    int i = 0;
    while (plaintext[i]) {
        char c = plaintext[i];
        if (isalpha(c)) {
            int idx = toupper(c) - 'A';
            ciphertext[i] = isupper(c) ? key[idx] : tolower(key[idx]);
        } else {
            ciphertext[i] = c;
        }
        i++;
    }
    ciphertext[i] = '\0';
    
    return SUCCESS;
}

int monoalphabetic_decrypt(const char* ciphertext, const char* key, char* plaintext) {
    if (!ciphertext || !key || !plaintext) return ERROR_INVALID_INPUT;
    if (strlen(key) != 26) return ERROR_INVALID_KEY;
    
    int i = 0;
    while (ciphertext[i]) {
        char c = ciphertext[i];
        if (isalpha(c)) {
            char upper_c = toupper(c);
            int idx;
            for (idx = 0; idx < 26; idx++) {
                if (key[idx] == upper_c) break;
            }
            plaintext[i] = isupper(c) ? ('A' + idx) : ('a' + idx);
        } else {
            plaintext[i] = c;
        }
        i++;
    }
    plaintext[i] = '\0';
    
    return SUCCESS;
}

void monoalphabetic_cipher_menu(void) {
    char* input = get_user_input("Enter text: ", MAX_INPUT_LENGTH);
    if (!input) {
        printf("Error: Failed to get input\n");
        return;
    }

    printf("\nKey options:\n");
    printf("1. Generate random key\n");
    printf("2. Enter custom key\n");
    printf("Choice: ");
    
    int key_choice;
    scanf("%d", &key_choice);
    clear_input_buffer();
    
    char* key;
    if (key_choice == 1) {
        key = generate_random_key();
        printf("Generated key: %s\n", key);
    } else {
        key = get_user_input("Enter 26-letter key (A-Z): ", 27);
        str_to_upper(key);
    }

    if (!key || strlen(key) != 26) {
        printf("Error: Invalid key\n");
        free(input);
        free(key);
        return;
    }

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
        status = monoalphabetic_encrypt(input, key, result);
        printf("\nEncrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else if (choice == 2) {
        status = monoalphabetic_decrypt(input, key, result);
        printf("\nDecrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else {
        printf("Invalid choice\n");
    }

    free(input);
    free(key);
    free(result);
}
