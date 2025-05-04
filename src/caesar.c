#include "caesar.h"

int caesar_encrypt(const char* plaintext, int shift, char* ciphertext) {
    if (!plaintext || !ciphertext) return ERROR_INVALID_INPUT;
    
    shift = ((shift % 26) + 26) % 26;
    
    int i = 0;
    while (plaintext[i]) {
        char c = plaintext[i];
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            ciphertext[i] = (c - base + shift) % 26 + base;
        } else {
            ciphertext[i] = c;
        }
        i++;
    }
    ciphertext[i] = '\0';
    
    return SUCCESS;
}

int caesar_decrypt(const char* ciphertext, int shift, char* plaintext) {
    return caesar_encrypt(ciphertext, -shift, plaintext);
}

void caesar_cipher_menu(void) {
    char* input = get_user_input("Enter text: ", MAX_INPUT_LENGTH);
    if (!input) {
        printf("Error: Failed to get input\n");
        return;
    }

    char* shift_str = get_user_input("Enter shift (0-25): ", 4);
    if (!shift_str) {
        free(input);
        printf("Error: Failed to get shift value\n");
        return;
    }

    int shift = atoi(shift_str);
    free(shift_str);

    if (shift < 0 || shift > 25) {
        printf("Error: Shift must be between 0 and 25\n");
        free(input);
        return;
    }

    char* result = malloc(strlen(input) + 1);
    if (!result) {
        printf("Error: Memory allocation failed\n");
        free(input);
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
        status = caesar_encrypt(input, shift, result);
        printf("\nEncrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else if (choice == 2) {
        status = caesar_decrypt(input, shift, result);
        printf("\nDecrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else {
        printf("Invalid choice\n");
    }

    free(input);
    free(result);
}
